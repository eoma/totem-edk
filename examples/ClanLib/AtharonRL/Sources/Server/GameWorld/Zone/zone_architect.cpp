#include "precomp.h"
#include "zone_architect.h"
#include "zone_map.h"
#include "zone_map_section.h"

//#include "Engine/Server/GameWorld/LibTCOD/mersenne.hpp"

using namespace clan;

/*bool BspTraversalListener::visitNode(TCODBsp *node, void *userData)
{
	if ( node->isLeaf() ) 
	{
		int x,y,w,h;
		int room_min_size = architect->get_room_min_size();
		
		// dig a room
		TCODRandom *rng = architect->get_rng();
		w = rng->getInt(room_min_size, node->w-2);
		h = rng->getInt(room_min_size, node->h-2);
		x = rng->getInt(node->x+1, node->x+node->w-w-1);
		y = rng->getInt(node->y+1, node->y+node->h-h-1);

		//TODO: Should change this, so that we send in a position and the boundaries,
		//		instead of sending in a rectangle...
		architect->create_room(room_num == 0, Vec2i(x, y), Vec2i(x+w-1, y+h-1));

		if ( room_num != 0 ) 
		{
			// dig a corridor from last room
			architect->create_corridor(Vec2i(last_x,last_y), Vec2i(x+w/2,last_y), Vec2i(x+w/2,last_y), Vec2i(x+w/2,y+h/2));
		}
		else
		{
			last_x=x+w/2;
            last_y=y+h/2;
            room_num++;
        }
        return true;
	}
	return false;
}
*/
ZoneArchitect::ZoneArchitect()
	: room_min_size(3), room_max_size(20), gameobjects(nullptr)
{
}

ZoneArchitect::~ZoneArchitect()
{
}

int ZoneArchitect::generate(int seed, const ZoneMapPtr &map)
{
	//if(seed == -1)
	//	seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);

	this->map = map;
	/*rng = new TCODRandom(seed, TCOD_RNG_CMWC);

	TCODBsp bsp(0,0,map->get_width(),map->get_height());
    bsp.splitRecursive(rng, 8, room_min_size, room_max_size, 1.75f, 1.75f);
    BspTraversalListener listener(this);
    bsp.traverseInvertedLevelOrder(&listener, nullptr);*/

	for(int y = 0; y < map->get_height(); y++)
	{
		for(int x = 0; x < map->get_width(); x++)
		{
			//50% walls
			if(rand()%2 == 1)
				map->set_properties(Vec2i(x,y), true, true, true);
		}
	}

	return seed;
}

int ZoneArchitect::generate(int seed, const ZoneMapPtr &map, GameObjectContainer *&gameobjects)
{
	this->gameobjects = gameobjects;
	return generate(seed, map);
}

void ZoneArchitect::dig(const Vec2i &position, const Vec2i &bounds, std::vector<ServerMapTilePtr> &tiles)
{
	//TODO: Should change this logic, so that the concept of a position and boundaries
	//		are actually true. Right now it follows more the concept of a rectangle...
	int x1 = position.x;
	int x2 = bounds.x;
	int y1 = position.y;
	int y2 = bounds.y;

    if ( x2 < x1 ) 
	{
        int tmp=x2;
        x2=x1;
        x1=tmp;
    }
    if ( y2 < y1 ) 
	{
        int tmp=y2;
        y2=y1;
        y1=tmp;
    }

	for (int tilex=x1; tilex <= x2; tilex++) 
	{
		for (int tiley=y1; tiley <= y2; tiley++) 
		{
			auto tile = map->set_properties(Vec2i(tilex,tiley),true,true, true);
			if(tile != nullptr)
				tiles.push_back(tile);
		}
	}
}

void ZoneArchitect::create_room(bool first, const Vec2i &position, const Vec2i &bounds)
{
	/*int x1 = position.x;
	int x2 = bounds.x;
	int y1 = position.y;
	int y2 = bounds.y;*/

	std::vector<ServerMapTilePtr> tiles;
    dig (position,bounds, tiles);

	//Register room as a map section
	auto section = std::make_shared<ZoneMapSection>("room", position, bounds);
	section->add(tiles);
	map->add_section(section);

	if(gameobjects == nullptr)
		return;

	//TODO: We need to add some entities to the room here, like items, monsters, etc
}

void ZoneArchitect::create_corridor(const clan::Vec2i &position1, const clan::Vec2i &bounds1, const clan::Vec2i &position2, const clan::Vec2i &bounds2)
{
	//TODO: Should probably merge this into a single corridor section, though I'm not entirely sure about exactly what's done here... :P
	std::vector<ServerMapTilePtr> tiles1;
	dig(position1, bounds1, tiles1);
	auto section1 = std::make_shared<ZoneMapSection>("corridor", position1, bounds1);
	section1->add(tiles1);
	map->add_section(section1);

	std::vector<ServerMapTilePtr> tiles2;
	dig(position2, bounds2, tiles2);
	auto section2 = std::make_shared<ZoneMapSection>("corridor", position1, bounds1);
	section2->add(tiles2);
	map->add_section(section2);

	if(gameobjects == nullptr)
		return;

	//TODO: We might want to add some entities to the corridor here, like items, monsters, etc
}
