#pragma once

#include "server_map_tile.h"

class FOVAlgorithmInterface
{
public:
	FOVAlgorithmInterface() {}
	virtual ~FOVAlgorithmInterface() {}

	virtual std::vector<ServerMapTilePtr> find_visible_tiles(const std::vector<ServerMapTilePtr> &map, const clan::Vec2i &map_size, 
														const clan::Vec2i &source_position, int max_radius, bool light_walls) = 0;

protected:
	int to_index(const clan::Vec2i &position, int width) const { return position.y*width + position.x; }
};