#pragma once

#include <memory>
#include "Engine/Server/GameWorld/server_map_tile.h"

class ServerMapTile; typedef std::shared_ptr<ServerMapTile> ServerMapTilePtr;

class ZoneMapSection
{
public:
	ZoneMapSection(const std::string &name, const clan::Point &position, const clan::Point &bounds, const std::string &prefab = std::string());
	~ZoneMapSection();

	void add(const ServerMapTilePtr &tile);
	void add(const std::vector<ServerMapTilePtr> &tiles);
	void remove(const ServerMapTilePtr &tile);

	const std::string &get_name() const { return name; }
	const std::string &get_prefab() const { return prefab; }
	const clan::Point &get_position() const { return position; }
	const clan::Point &get_bounds() const { return bounds; }

private:
	void recalculate_bounds();

	std::string name;
	std::string prefab;

	std::vector<ServerMapTilePtr> data; //Should this be replaced by a ZoneMapTileContainer?

	clan::Point position;				//How is position defined? Center of tiles in the group? A corner?
	clan::Point bounds;					//This is the bounds of the tile group, and will need to be dynamic based on add/remove of tiles...
};
