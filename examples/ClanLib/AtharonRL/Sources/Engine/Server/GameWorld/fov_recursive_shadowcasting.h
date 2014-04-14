
#pragma once

#include "fov_algorithm_interface.h"

class FOVRecursiveShadowcasting : public FOVAlgorithmInterface
{
public:
	FOVRecursiveShadowcasting();
	virtual ~FOVRecursiveShadowcasting();

	std::vector<ServerMapTilePtr> find_visible_tiles(const std::vector<ServerMapTilePtr> &map, const clan::Vec2i &map_size, 
												const clan::Vec2i &source_position, int max_radius, bool light_walls) override;

protected:
	void cast_light(const std::vector<ServerMapTilePtr> &map, const clan::Vec2i &map_size, const clan::Vec2i &source_position, 
				int row, float start, float end, 
				int radius, int r2, 
				int xx, int xy, int yx, int yy, 
				int id, bool light_walls,
				std::vector<ServerMapTilePtr> &vicinity_map);
};