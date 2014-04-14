
#pragma once

#include "server_map_tile.h"

class FieldOfView
{
public:
	enum FieldOfViewAlgorithm
	{
		FOV_SHADOWCASTING = 0
	};
	static std::vector<ServerMapTilePtr> find_visible_tiles(	const std::vector<ServerMapTilePtr> &map, const clan::Vec2i &map_size, 
														const clan::Vec2i &source_position, int max_radius = 0, 
														bool light_walls = true, FieldOfViewAlgorithm algorithm = FOV_SHADOWCASTING);
};
