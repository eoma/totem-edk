#include "precomp.h"
#include "client_vicinity_map.h"
#include "client_zone.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

ClientVicinityMap::ClientVicinityMap(ClientZone *zone, const LayerManagerPtr &layer_manager)
: player_gameobject_id(0), zone(zone), layer_manager(layer_manager)
{
	netevents.func_event(STC_MAP_CREATE).set(this, &ClientVicinityMap::on_net_event_map_create);
	netevents.func_event(STC_MAP_UPDATE).set(this, &ClientVicinityMap::on_net_event_map_update);
	netevents.func_event(STC_MAP_DESTROY).set(this, &ClientVicinityMap::on_net_event_map_destroy);
	netevents.func_event(STC_MAP_EVENT).set(this, &ClientVicinityMap::on_net_event_map_event);
}

ClientVicinityMap::~ClientVicinityMap()
{
	clear();
}

bool ClientVicinityMap::dispatch_net_event(const NetGameEvent &event)
{
	return netevents.dispatch(event);
}

void ClientVicinityMap::on_net_event_map_create(const NetGameEvent &e)
{
	/*int a = 0;

	int id = e.get_argument(a++);
	std::string name = e.get_argument(a++);

	cl_log_event("Game", "Creating object %1 %2", id, name);

	auto gameobject = std::shared_ptr<ClientGameObject>(new ClientGameObject(zone, id, name));

	unsigned int component_count = e.get_argument(a++);
	for(size_t c = 0; c < component_count; ++c)
	{
		std::string component_type = e.get_argument(a++);
		std::string component_name = e.get_argument(a++);

		//TODO: Should fix so that entire engine uses shared_ptr instead of raw pointer!!!
		component_factory->create_and_add_component(gameobject.get(), component_type, component_name);
		cl_log_event("Game", "- Component %1 %2", component_type, component_name);

		unsigned int property_count = e.get_argument(a++);
		for(size_t p = 0; p < property_count; ++p)
		{
			int property_type = e.get_argument(a++);
			std::string property_name = e.get_argument(a++);
			std::string property_value = e.get_argument(a++);

			PropertySerializer::create_and_add_property(gameobject.get(), property_type, property_name, property_value);
			cl_log_event("Game", "  - Property %1 %2", property_name, property_value);
		}
	}

	add_object(gameobject);

	if(player_gameobject_id == id)
		zone->set_camera_target(gameobject);*/
}

void ClientVicinityMap::on_net_event_map_update(const NetGameEvent &e)
{
	//TODO: Should really ensure reuse of tiles here instead of clearing the entire list,
	//		which will cause a delete on all the tiles, then instanciate new tiles based
	//		on the event arguments...
	int a = 0;
	visible_tiles.clear();

	cl_log_event("Game", "Updating map");

	unsigned int tile_count = e.get_argument(a++);
	for(unsigned int i = 0; i < tile_count; ++i)
	{
		Vec2i tile_position = Vec2i(e.get_argument(a++), e.get_argument(a++));
		int walkable = e.get_argument(a++);

		auto tile = std::make_shared<ClientMapTile>(tile_position, (bool)walkable, zone, layer_manager);
		visible_tiles.push_back(tile);
	}
	/*int a = 0;

	int id = e.get_argument(a++);

	cl_log_event("Game", "Updating object %1", id);

	auto gameobject = find_gameobject(id);
	if(gameobject)
	{
		unsigned int property_count = e.get_argument(a++);
		for(size_t p = 0; p < property_count; ++p)
		{
			int property_type = e.get_argument(a++);
			std::string property_name = e.get_argument(a++);
			std::string property_value = e.get_argument(a++);

			PropertySerializer::create_and_add_property(gameobject.get(), property_type, property_name, property_value);
			cl_log_event("Game", "- Property %1 %2", property_name, property_value);
		}
	}*/
}

void ClientVicinityMap::on_net_event_map_destroy(const NetGameEvent &e)
{
	/*int id = e.get_argument(0);
	auto gameobject = find_gameobject(id);
	if(gameobject)
	{
		//Erases the gameobject from the visible_objects list in the update function
		gameobject->destroy();
	}*/
}

void ClientVicinityMap::on_net_event_map_event(const NetGameEvent &e)
{
	/*int gameobject_id = e.get_argument(0);

	bool handled_event = false;

	auto gameobject = find_gameobject(gameobject_id);
	if(gameobject)
	{
		std::string event_name = e.get_argument(1);
		NetGameEvent gameobject_event(event_name);
		for (size_t i = 2; i < e.get_argument_count(); i++)
			gameobject_event.add_argument(e.get_argument(i));

		handled_event = gameobject->dispatch_net_event(gameobject_event);
	}

	if (!handled_event)
		cl_log_event("Network", "Unhandled gameobject event: %1", e.to_string());*/
}

void ClientVicinityMap::clear()
{
	visible_tiles.clear(); //Shared ptr only needs to clear list
}

void ClientVicinityMap::update(float time_elapsed)
{
	for (size_t i = 0; i < visible_tiles.size(); i++)
	{
		visible_tiles[i]->update(time_elapsed);
	}
}

void ClientVicinityMap::draw(const ClientCameraPtr &camera)
{
	for (size_t i = 0; i < visible_tiles.size(); i++)
	{
		visible_tiles[i]->draw(camera);
	}
}
