#pragma once

#include "Engine/Server/GameWorld/server_map_tile.h"

class Player;
class Zone;
class ZoneMap; typedef std::shared_ptr<ZoneMap> ZoneMapPtr;

class ZoneVicinityMap
{
// Construction:
public:
	ZoneVicinityMap(const ZoneMapPtr &map, Player *player, clan::NetGameConnection *connection);
	~ZoneVicinityMap();

// Attributes:
public:
	//Checks whether this tile can be seen by the player
	bool is_in_vicinity(const clan::Vec2i &position) const;

	//Checks whether this tile has already been discovered by the player
	bool is_explored(const clan::Vec2i &position) const;

	//After a call to compute_vicinity, this list will be filled with all the new tiles you discovered.
	//Might be useful to access for some logical component to reward experience for all the new tiles
	//you discovered, etc.
	const std::vector<ServerMapTilePtr> &get_new_visible_tiles() const { return new_visible_tiles; }

// Operations:
public:
	void update(float time_elapsed);
	void sync_dirty_tiles();
	void clear_dirty_tiles();

	//Computes new list of visible tiles, and marks any newly discovered tiles
	//that is then stored in tile memory
	void compute_vicinity();

// Implementation:
private:
	bool valid(const clan::Vec2i &position) const;

	ZoneMapPtr map;
	Player *player;
	clan::NetGameConnection *connection;

	std::vector<ServerMapTilePtr> tile_memory; //Remember all tiles you've ever visited in this zone
	std::vector<ServerMapTilePtr> visible_tiles;	//List of all tiles currently in your vicinity
	std::vector<ServerMapTilePtr> new_visible_tiles; //List of all new tiles that you've never seen before
	
	//Hacks
	bool first;
};
