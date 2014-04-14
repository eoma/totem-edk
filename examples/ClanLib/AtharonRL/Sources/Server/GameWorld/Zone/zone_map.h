#pragma once

#include "Engine/Server/GameWorld/server_map_tile.h"

class ZoneMapSection; typedef std::shared_ptr<ZoneMapSection> ZoneMapSectionPtr;

class ZoneMap
{
public:
	ZoneMap(clan::SqliteConnection &db, const clan::Vec2i &size);
	~ZoneMap();

	ServerMapTilePtr set_properties(const clan::Vec2i &position, bool transparent, bool walkable, bool architected);

	int get_width() const { return size.x; }
	int get_height() const { return size.y; }

	const std::vector<ServerMapTilePtr> &get_all_tiles() const { return tiles; }

	void add_section(const ZoneMapSectionPtr &section);
	void remove_section(const ZoneMapSectionPtr &section);

private:
	bool valid(const clan::Vec2i &position) const;
	unsigned int to_index(const clan::Vec2i &position) const;

	void clear();

	clan::Vec2i size;

	std::vector<ServerMapTilePtr> tiles;
	std::vector<ServerMapTilePtr> dirty_tiles;

	std::vector<ZoneMapSectionPtr> groups;

	clan::SqliteConnection &db;
};
