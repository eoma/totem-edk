
#include "precomp.h"
#include "field_of_view.h"
#include "fov_recursive_shadowcasting.h"

using namespace clan;

std::vector<ServerMapTilePtr> FieldOfView::find_visible_tiles(const std::vector<ServerMapTilePtr> &map, const clan::Vec2i &map_size, 
														const clan::Vec2i &source_position, int max_radius, bool light_walls, 
														FieldOfViewAlgorithm algorithm)
{
	switch(algorithm)
	{
	case FOV_SHADOWCASTING:
		{
			FOVRecursiveShadowcasting fov;
			return fov.find_visible_tiles(map, map_size, source_position, max_radius, light_walls);
		}break;
	default:
		return std::vector<ServerMapTilePtr>();
	};
}
