#include "precomp.h"
#include "zone_player_manager.h"
#include "GameWorld/player.h"
#include "GameWorld/character.h"
#include "GameWorld/server_gameobject.h"
#include "GameWorld/gameobject_container.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

ZonePlayerManager::ZonePlayerManager()
{
}

ZonePlayerManager::~ZonePlayerManager()
{
}

Player *ZonePlayerManager::find_player_with_gameobject(ServerGameObject *gameobject) const
{
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		Player *player = it->first;
		Character *character = player->get_character();
		if(character->get_owner_gameobject() == gameobject)
			return player;
	}
	return nullptr;
}

void ZonePlayerManager::add_player(Player *player, const GameObjectContainerPtr &gameobjects, const ZoneMapPtr &map)
{
	ZoneVicinityObjects *vicinity_objects = new ZoneVicinityObjects(player->get_connection());

	// TODO: For now just add all objects to visibility
	auto &objects = gameobjects->get_all();
	for (auto it = objects.begin(); it != objects.end(); ++it)
		vicinity_objects->add_gameobject((*it));

	ZoneVicinityMap *vicinity_map = new ZoneVicinityMap(map, player, player->get_connection());
	ZoneVicinity *vicinity = new ZoneVicinity(vicinity_map, vicinity_objects);

	players[player] = vicinity;

	NetGameEvent zone_event(STC_ZONE);
	player->send_event(zone_event);
}

void ZonePlayerManager::remove_player(Player *player)
{
	auto it = players.find(player);
	if (it != players.end())
	{
		ZoneVicinity *vicinity = it->second;
		delete vicinity;
		players.erase(it);
	}
}

void ZonePlayerManager::update(float time_elapsed)
{
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		it->second->map->update(time_elapsed);
	}
}

void ZonePlayerManager::notify_players_object_added(ServerGameObject *gameobject)
{
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		ZoneVicinityObjects *objects = it->second->objects;
		objects->add_gameobject(gameobject);
	}
}

void ZonePlayerManager::notify_players_object_removed(ServerGameObject *gameobject)
{
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		ZoneVicinityObjects *objects = it->second->objects;
		objects->remove_gameobject(gameobject);
	}
}

void ZonePlayerManager::notify_players_map_changed()
{
	/*for(auto it = players.begin(); it != players.end(); ++it)
	{
		ZoneVicinityMap *map = it->second->map;
		map->
	}*/
}

void ZonePlayerManager::save()
{
}

void ZonePlayerManager::sync()
{
	//Sync map
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		ZoneVicinityMap *vicinity_map = it->second->map;
		vicinity_map->sync_dirty_tiles();
	}

	for(auto it = players.begin(); it != players.end(); ++it)
	{
		ZoneVicinityMap *vicinity_map = it->second->map;
		vicinity_map->clear_dirty_tiles();
	}

	//Sync gameobjects
	for(auto it = players.begin(); it != players.end(); ++it)
	{
		ZoneVicinityObjects *objects = it->second->objects;
		objects->sync_dirty_gameobjects();
	}

	for(auto it = players.begin(); it != players.end(); ++it)
	{
		ZoneVicinityObjects *objects = it->second->objects;
		objects->clear_dirty_gameobjects();
	}
}
