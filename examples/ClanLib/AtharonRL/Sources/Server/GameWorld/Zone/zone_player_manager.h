#pragma once

//TODO: This class should replace a lot of player handling that currently happens in Zone.
//		Should replace std::map<ServerPlayer *, ZoneVicinity *> players, and implement all
//		current interactions Zone has with this map.

#include "zone_vicinity_objects.h"
#include "zone_vicinity_map.h"

class Player;
class GameObjectContainer; typedef std::shared_ptr<GameObjectContainer> GameObjectContainerPtr;
class ServerGameObject;
class ZoneMap; typedef std::shared_ptr<ZoneMap> ZoneMapPtr;

class ZonePlayerManager
{
public:
	ZonePlayerManager();
	~ZonePlayerManager();

	Player *find_player_with_gameobject(ServerGameObject *gameobject) const;

	void add_player(Player *player, const GameObjectContainerPtr &gameobjects, const ZoneMapPtr &map);
	void remove_player(Player *player);

	void update(float time_elapsed);

	void notify_players_object_added(ServerGameObject *gameobject);
	void notify_players_object_removed(ServerGameObject *gameobject);
	void notify_players_map_changed();

	void sync();
	void save();

private:
	struct ZoneVicinity
	{
		ZoneVicinityObjects *objects;
		ZoneVicinityMap *map;

		ZoneVicinity(ZoneVicinityMap *map, ZoneVicinityObjects *objects) : map(map), objects(objects) {}
		~ZoneVicinity() { delete map; delete objects; }
	};
	std::map<Player *, ZoneVicinity *> players;
};