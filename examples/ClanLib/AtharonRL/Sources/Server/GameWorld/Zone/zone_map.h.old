#pragma once

#include <libtcod.hpp>
#include "Engine/Server/GameWorld/server_map_tile.h"

class ServerPlayer;
class ZoneVicinityObjects;

class ZoneMap
{
public:
	ZoneMap(clan::SqliteConnection &db, const clan::Vec2i &size);
	~ZoneMap();

	void clear();
	void update(float time_elapsed);

	int get_width() const { return size.x; }
	int get_height() const { return size.y; }
	const clan::Vec2i &get_size() const { return size; }

	//Position of the tile
	//Transparent: Can we see tiles behind this tile?
	//Walkable: Can we step on this tile?
	//Architected: Is the call to this function invoked from the procedural architect? 
	//			   Or is it a player/monster/item/Other event?
	void set_properties(const clan::Vec2i &position, bool transparent, bool walkable, bool architected = false);

	//Is wall simply checks if the tile is walkable
	bool is_wall(const clan::Vec2i &position) const;

	//Can walk checks for is wall, but in addition checks for
	//potential game objects that might hinder/block us from
	//stepping onto it
	bool can_walk(const clan::Vec2i &position) const;

	const std::vector<ServerMapTilePtr> &get_dirty_tiles() const { return dirty_tiles; }
	void clear_dirty_tiles();

	void load_from_database();
	void save_dirty_tiles();



protected:
	friend class ZoneVicinityMap;
	const std::vector<ServerMapTilePtr> &get_tiles() const { return tiles; }

private:
	bool valid(const clan::Vec2i &position) const;
	unsigned int to_index(const clan::Vec2i &position) const;

	clan::Vec2i size;

	std::vector<ServerMapTilePtr> tiles;
	std::vector<ServerMapTilePtr> dirty_tiles;

	clan::SqliteConnection &db;
};
