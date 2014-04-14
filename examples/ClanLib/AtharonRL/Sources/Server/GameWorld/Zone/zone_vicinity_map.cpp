#include "precomp.h"
#include "zone_vicinity_map.h"
#include "zone.h"
#include "zone_map.h"
#include "GameWorld/player.h"
#include "GameWorld/character.h"
#include "GameWorld/server_gameobject.h"
#include "Engine/Common/Network/netevents.h"
#include "Engine/Server/GameWorld/field_of_view.h"
#include <algorithm>

using namespace Totem;
using namespace clan;

ZoneVicinityMap::ZoneVicinityMap(const ZoneMapPtr &map, Player *player, NetGameConnection *connection)
: map(map), player(player), connection(connection), first(true)
{
}

ZoneVicinityMap::~ZoneVicinityMap()
{
	visible_tiles.clear();
}

bool ZoneVicinityMap::valid(const Vec2i &position) const
{
	if(position.x < 0 || position.y < 0 || position.x >= map->get_width() || position.y >= map->get_height())
		return false;
	else
		return true;
}

void ZoneVicinityMap::update(float time_elapsed)
{
	//Call this from here for now...
	compute_vicinity();
}

void ZoneVicinityMap::clear_dirty_tiles()
{
	/*for (unsigned int i = 0; i < visible_tiles.size(); i++)
	{
		auto tile = visible_map_tiles[i];
	}*/
}

void ZoneVicinityMap::sync_dirty_tiles()
{
	if(visible_tiles.empty())
		return;
		
	if( !first )
		return;
	else
		first = false;

	NetGameEvent create_event(STC_MAP_UPDATE);
	unsigned int visible_tiles_count = visible_tiles.size();
	create_event.add_argument(visible_tiles_count);

	//Iterate over all visible tiles
	for (unsigned int i = 0; i < visible_tiles_count; i++)
	{
		auto tile = visible_tiles[i];
		create_event.add_argument(tile->get_position().x);
		create_event.add_argument(tile->get_position().y);
		create_event.add_argument((int)tile->is_walkable());
		//TODO: Need some more information about the tile... like what background color is it?
		//		does it have a foreground color and a character associated with it? etc...
	}

	connection->send_event(create_event);
}

void ZoneVicinityMap::compute_vicinity()
{
	//Clear all visibility calculations from last time
	visible_tiles.clear();
	new_visible_tiles.clear();

	//Make sure we have a character
	auto character = player->get_character();
	if( character == 0 )
		return;

	//Make sure we have a game object
	auto go = character->get_owner_gameobject();
	if( go == 0 )
		return;

	//Look for the player's position
	Vec2i position = Vec2i(0,0);
	if(go->hasProperty("Position"))
		position = go->get<Vec2i>("Position").get();

	//Look for the player's view radius
	int radius = 0;
	if(go->hasProperty("ViewRadius"))
		radius = go->get<int>("ViewRadius").get();

	//Find all visible tiles
	//visible_tiles = FieldOfView::find_visible_tiles(map->get_tiles(), map->get_size(), position, radius, true, FieldOfView::FOV_SHADOWCASTING);
	visible_tiles = map->get_all_tiles();

	//Check if we discovered some new tiles
	for(unsigned int i = 0; i < visible_tiles.size(); i++)
	{
		auto tile = visible_tiles[i];

		bool is_new_tile = true;
		for(unsigned int j = 0; j < tile_memory.size(); j++)
		{
			if(tile == tile_memory[j])
			{
				is_new_tile = false;
				break;
			}
		}

		if(is_new_tile)
			new_visible_tiles.push_back(tile);
	}

	//Now store all new tiles in memory
	for(unsigned int i = 0; i < new_visible_tiles.size(); i++)
	{
		auto tile = new_visible_tiles[i];
		tile_memory.push_back(tile);
	}
}

bool ZoneVicinityMap::is_in_vicinity(const clan::Vec2i &position) const
{
	if( !valid(position) )
		return false;
	
	for(unsigned int i = 0; i < visible_tiles.size(); i++)
	{
		auto tile = visible_tiles[i];
		if(tile->get_position() == position)
			return true;
	}
	return false;
}

bool ZoneVicinityMap::is_explored(const clan::Vec2i &position) const
{
	for(unsigned int i = 0; i < tile_memory.size(); i++)
	{
		if(tile_memory[i]->get_position() == position)
			return true;
	}
	return false;
}
