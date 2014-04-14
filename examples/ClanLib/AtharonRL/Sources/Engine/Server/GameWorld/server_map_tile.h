#pragma once

#include "Engine/Common/GameWorld/map_tile_interface.h"

class ServerMapTile; typedef std::shared_ptr<ServerMapTile> ServerMapTilePtr;

class ServerMapTile : public MapTileInterface
{
public:
	ServerMapTile(const clan::Vec2i &position, bool transparent, bool walkable) : MapTileInterface(position), transparent(transparent), walkable(walkable) {}
	virtual ~ServerMapTile() {}
	
	bool is_transparent() const { return transparent; }
	bool is_walkable() const { return walkable; }

	void set_transparent(bool transparent = true) { this->transparent = transparent; }
	void set_walkable(bool walkable = true) { this->walkable = walkable; }

protected:
	bool transparent;
	bool walkable;
};
