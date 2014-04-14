#pragma once

#include <memory>

class MapTileInterface; typedef std::shared_ptr<MapTileInterface> MapTileInterfacePtr;

class MapTileInterface
{
public:
	MapTileInterface(const clan::Vec2i &position) : position(position) {}
	virtual ~MapTileInterface() {}
	
	const clan::Vec2i &get_position() const { return position; }

	void set_position(const clan::Vec2i &position) { this->position = position; }

protected:
	clan::Vec2i position;
};
