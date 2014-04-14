#include "precomp.h"
#include "zone_manager.h"
#include "zone.h"
#include "zone_map.h"
#include "zone_architect.h"
#include "GameWorld/player.h"
#include "GameWorld/server_gameobject.h"
#include "GameWorld/gameobject_container.h"
#include "GameWorld/Components/network_receiver.h"
#include "Database/database_zone_instances.h"
#include "Database/database_gameobjects.h"
#include "Database/database_gameobject_containers.h"
#include "Engine/Common/Network/netevents.h"

using namespace Totem;
using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

ZoneManager::ZoneManager(SqliteConnection &db)
: db(db)
{
	netevents.func_event(CTS_OBJECT_EVENT).set(this, &ZoneManager::on_net_event_object_event);
}

ZoneManager::~ZoneManager()
{
	for (size_t i = 0; i < zones.size(); i++)
		delete zones[i];
	zones.clear();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

ServerGameObject *ZoneManager::find_gameobject(int gameobject_id) const
{
	for(size_t i = 0; i < zones.size(); ++i)
	{
		ServerGameObject *gameobject = zones[i]->find_gameobject(gameobject_id);
		if(gameobject != 0)
			return gameobject;
	}	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

Zone *ZoneManager::get_or_load_zone(int zone_id)
{
	Zone *zone = find_zone(zone_id);
	if(zone == nullptr)
	{
		if(DatabaseZoneInstances::is_zone_instantiated(db, zone_id))
		{
			zone = load_zone(zone_id);
		}
		else
		{
			zone = instansiate_zone();
		}
	}
	return zone;
}

void ZoneManager::update()
{
	for (auto it = zones.begin(); it != zones.end(); ++it)
	{
		Zone *zone = (*it);
		zone->update();	
	}
}

bool ZoneManager::dispatch_net_event(const NetGameEvent &event, Player *player)
{
	return netevents.dispatch(event, player);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void ZoneManager::on_net_event_object_event(const NetGameEvent &e, Player *player)
{
	int gameobject_id = e.get_argument(0);

	bool handled_event = false;

	ServerGameObject *gameobject = find_gameobject(gameobject_id);
	if(gameobject)
	{
		std::string event_name = e.get_argument(1);
		NetGameEvent gameobject_event(event_name);
		for (size_t i = 2; i < e.get_argument_count(); i++)
			gameobject_event.add_argument(e.get_argument(i));

		auto &components = gameobject->getComponents();
		for(size_t i = 0; i < components.size(); ++i)
		{
			NetworkReceiver *component = dynamic_cast<NetworkReceiver *> (components[i].get());
			if(component)
			{
				handled_event |= component->dispatch_net_event(gameobject_event, player);
			}
		}
	}

	if (!handled_event)
		cl_log_event("Network", "Unhandled gameobject event: %1", e.to_string());
}

Zone *ZoneManager::find_zone(int zone_id)
{
	for (size_t i = 0; i < zones.size(); ++i)
	{
		if(zones[i]->get_id() == zone_id)
			return zones[i];
	}
	return 0;
}

Zone *ZoneManager::load_zone(int zone_id)
{
	cl_log_event("Zones", "Loading existing zone %1", zone_id);

	DatabaseZoneInstances::ZoneInstanceInfo zone_instance = DatabaseZoneInstances::get_info(db, zone_id);

	//Create content objects
	ZoneMapPtr map = std::make_shared<ZoneMap>(db, Vec2i(40,40));
	ComponentFactoryPtr component_factory = std::make_shared<ComponentFactory>();
	GameObjectContainerPtr gameobjects = std::make_shared<GameObjectContainer>(db, zone_instance.gameobject_container_id);

	//Generate map content
	ZoneArchitect architect;
	int generated_seed = architect.generate(zone_instance.generation_seed, map);

	//Load persisted gameobjects
	gameobjects->load_from_database(component_factory);

	//Create the zone
	Zone *zone = new Zone(db, zone_id, gameobjects, map, component_factory);
	zones.push_back(zone);

	if(generated_seed != zone_instance.generation_seed)
	{
		//This should never happen when loading... well... it does for now, but it the future this shouldn't happen :P 
	}

	cl_log_event("Zones", "Zone %1 loaded", zone_id);

	return zone;
}

Zone *ZoneManager::instansiate_zone()
{
	//TODO...
	return nullptr;
}
