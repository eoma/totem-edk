#include "precomp.h"
#include "zone.h"
#include "zone_map.h"
#include "GameWorld/player.h"
#include "GameWorld/character.h"
#include "GameWorld/server_gameobject.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

Zone::Zone(SqliteConnection &db, int zone_id, const GameObjectContainerPtr &gameobjects, const ZoneMapPtr &map, const ComponentFactoryPtr &component_factory)
: zone_id(zone_id), gameobjects(gameobjects), map(map), component_factory(component_factory)
{
}

Zone::~Zone()
{
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int Zone::get_id() const
{
	return zone_id;
}

ServerGameObject *Zone::find_gameobject(int gameobject_id) const
{
	return gameobjects->find(gameobject_id);
}

Player *Zone::find_player_with_gameobject(ServerGameObject *gameobject) const
{
	return players.find_player_with_gameobject(gameobject);
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void Zone::add_player(Player *player)
{
	players.add_player(player, gameobjects, map);
}

void Zone::remove_player(Player *player)
{
	players.remove_player(player);
}

void Zone::add_gameobject(ServerGameObject *gameobject)
{
	if(gameobjects->add(gameobject))
	{
		notify_players_object_added(gameobject);
	}
}

void Zone::remove_gameobject(ServerGameObject *gameobject)
{
	if(gameobjects->remove(gameobject))
	{
		notify_players_object_removed(gameobject);
	}
}

void Zone::set_gameobject_inactive(ServerGameObject *gameobject)
{
	if(gameobjects->set_inactive(gameobject))
	{
		notify_players_object_removed(gameobject);
	}
}

void Zone::update()
{
	int ticks = tick_timer.update();

	tick(ticks * tick_timer.get_tick_length());

//	for (int i = 0; i < ticks; i++)
//		tick(tick_timer.get_tick_length());

//	static float total_time = 0;
//	total_time += time_elapsed;
	//	if(total_time > 10.0f)
	{
		players.sync();
		save();
//		total_time = 0.0f;
	}
}

void Zone::tick(float time_elapsed)
{
	//map->update(time_elapsed);
	players.update(time_elapsed);
	gameobjects->update(time_elapsed);
}

ServerGameObject *Zone::load_gameobject(int gameobject_id)
{
	ServerGameObject *gameobject = gameobjects->load_gameobject_from_database(gameobject_id, component_factory);
	return gameobject;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void Zone::notify_players_object_added(ServerGameObject *gameobject)
{
	players.notify_players_object_added(gameobject);
}

void Zone::notify_players_object_removed(ServerGameObject *gameobject)
{
	players.notify_players_object_removed(gameobject);
}

void Zone::notify_players_map_changed()
{
	players.notify_players_map_changed();
}

void Zone::save()
{
	//map->save_dirty_tiles();
	gameobjects->save_dirty_properties();
	players.save();
}
