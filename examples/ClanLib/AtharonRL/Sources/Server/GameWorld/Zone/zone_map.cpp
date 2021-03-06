#include "precomp.h"
#include "zone_map.h"
#include "zone_map_section.h"

using namespace clan;

ZoneMap::ZoneMap(clan::SqliteConnection &db, const clan::Vec2i &size)
	: db(db), size(size)
{
	tiles.reserve(size.x*size.y);
	clear();
}

ZoneMap::~ZoneMap()
{
}

void ZoneMap::clear()
{
	tiles.clear();
	for(int y = 0; y < size.y; y++)
	{
		for(int x = 0; x < size.x; x++)
		{
			//Clear to a completely opaque, non-walkable block of stone that we can carve our dungeons from.
			auto tile = std::make_shared<ServerMapTile>(Vec2i(x,y), false, false);
			tiles.push_back(tile);
		}
	}
}

bool ZoneMap::valid(const Vec2i &position) const
{
	if(position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y)
		return false;
	else
		return true;
}

unsigned int ZoneMap::to_index(const clan::Vec2i &position) const
{
	return position.y*size.x + position.x;
}

ServerMapTilePtr ZoneMap::set_properties(const Vec2i &position, bool transparent, bool walkable, bool architected)
{
	ServerMapTilePtr tile;

	if( !valid(position) )
		return nullptr;

	tile = tiles[ to_index(position) ];
	tile->set_position(position);
	tile->set_transparent(transparent);
	tile->set_walkable(walkable);

	//If this tile was architected, then it means it can be regenerated by the seed number of the zone, thus
	//we don't need to store the tile information (it can simply be regenerated from the seed. Otherwise,
	//this tile was changed by an event in the actual game, and the change should indeed be persisted, so that
	//the dungeon can be regenerated later with all the changes to it's features.
	if( !architected )
		dirty_tiles.push_back(tile);

	return tile;
}

void ZoneMap::add_section(const ZoneMapSectionPtr &section)
{
}

void ZoneMap::remove_section(const ZoneMapSectionPtr &section)
{
}
