
#include "precomp.h"
#include "zone_map_section.h"

ZoneMapSection::ZoneMapSection(const std::string &name, const clan::Point &position, const clan::Point &bounds, const std::string &prefab)
	: name(name), position(position), bounds(bounds), prefab(prefab)
{
}

ZoneMapSection::~ZoneMapSection()
{
}

void ZoneMapSection::add(const ServerMapTilePtr &tile)
{
}

void ZoneMapSection::add(const std::vector<ServerMapTilePtr> &tiles)
{
}

void ZoneMapSection::remove(const ServerMapTilePtr &tile)
{
}

void ZoneMapSection::recalculate_bounds()
{
}
