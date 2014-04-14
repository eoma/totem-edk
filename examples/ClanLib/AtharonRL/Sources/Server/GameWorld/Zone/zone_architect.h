#pragma once

#include "GameWorld/gameobject_container.h"
#include "Engine/Server/GameWorld/server_map_tile.h"

//#include "Engine/Server/GameWorld/LibTCOD/bsp.hpp"

#include <memory>

//class TCODRandom;

class ZoneMap; typedef std::shared_ptr<ZoneMap> ZoneMapPtr;

class ZoneArchitect;

/*class BspTraversalListener : public ITCODBspCallback
{
public:
	BspTraversalListener(ZoneArchitect *architect) : architect(architect), room_num(0) {}
	~BspTraversalListener() {}

	bool visitNode(TCODBsp *node, void * userData) override;

private:
	ZoneArchitect *architect; // a map to dig
	int room_num; // room number
    int last_x,last_y; // center of the last room
};*/


class ZoneArchitect
{
public:
	ZoneArchitect();
	~ZoneArchitect();

	//Only generate with entities the first time, later the entities
	//are stored and persisted in the database!
	int generate(int seed, const ZoneMapPtr &map);
	int generate(int seed, const ZoneMapPtr &map, GameObjectContainer *&gameobjects);

protected:
	friend class BspTraversalListener;
	
	void dig(const clan::Vec2i &position, const clan::Vec2i &bounds, std::vector<ServerMapTilePtr> &tiles);
    
	void create_room(bool first, const clan::Vec2i &position, const clan::Vec2i &bounds);
	void create_corridor(const clan::Vec2i &position1, const clan::Vec2i &bounds1, const clan::Vec2i &position2, const clan::Vec2i &bounds2);
	
	int get_room_min_size() const { return room_min_size; }
	int get_room_max_size() const { return room_max_size; }
	
	//TCODRandom *get_rng() const { return rng; }

private:
	int room_min_size;
	int room_max_size;
	ZoneMapPtr map;
	//TCODRandom *rng;
	GameObjectContainer *gameobjects;
};
