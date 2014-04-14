#pragma once

#include <libtcod.hpp>
#include <memory>

class Map;

class BspListener : public ITCODBspCallback
{
public:
	BspListener(Map *map);
	~BspListener();

	bool visitNode(TCODBsp *node, void * /*userData*/) override;

private:
	Map *map; // a map to dig
	int roomNum; // room number
    int lastx,lasty; // center of the last room
};
