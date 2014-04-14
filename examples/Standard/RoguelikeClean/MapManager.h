#pragma once

#include "Parser.h"

#include "Math/vec2.h"

#include <memory>
#include <vector>
#include <libtcod.hpp>

class MapManager
{
public:
	MapManager();
	~MapManager();

	enum MapLayer
	{
		LAYER_GROUND,
		LAYER_AIR,
		MAP_LAYER_COUNT
	};
	enum GenerationType
	{
		GENERATION_NONE,
		GENERATION_BSP,
		GENERATION_TYPE_COUNT
	};

	void initialize(const Parser::MapsInfo &mapsInfo);
	void generate();
	void generateEnemies();
	void render(const std::shared_ptr<TCODConsole> &canvas, MapLayer layer);

	bool isInFov(MapLayer layer, const clan::Vec2i &pos) const;
	bool isWall(MapLayer layer, const clan::Vec2i &pos) const;
    bool isExplored(MapLayer layer, const clan::Vec2i &pos) const;
    void computeFov(MapLayer layer, const clan::Vec2i &pos, int radius);

	void dig(MapLayer layer, clan::Vec2i bl, clan::Vec2i tr);
	void createRoom(MapLayer layer, bool first, const clan::Vec2i &bl, const clan::Vec2i &tr);

private:
	class Map;
	class MapBspListener : public ITCODBspCallback
	{
	public:
		MapBspListener(MapLayer layer) : _layer(layer), _roomNum(0) {}

		bool visitNode(TCODBsp *node, void *userData) override;

	private:
		MapLayer _layer;
		int _roomNum;
		clan::Vec2i _last;
	};

	class Map
	{
	public:
		Map(MapLayer layer, const clan::Vec2i &size) 
			: layer(layer), size(size) 
		{ 
			data = std::make_shared<TCODMap>(size.x, size.y); 
			tiles.resize(size.x*size.y, nullptr);
			for(unsigned int i = 0; i < tiles.size(); i++)
				tiles[i] = std::make_shared<Tile>();
		}
		~Map() {}

		struct Tile
		{
			bool explored;

			Tile() : explored(false) {}
		};

		const MapLayer layer;
		clan::Vec2i size;
		GenerationType generationType;
		std::shared_ptr<TCODMap> data;
		std::vector<std::shared_ptr<Tile>> tiles;
		std::vector<clan::Vec2i> enemyPositions;

		TCODColor wallInViewColor;
		TCODColor groundInViewColor;
		TCODColor wallInMemoryColor;
		TCODColor groundInMemoryColor;

	protected:
		friend class MapBspListener;
	};
	std::vector<std::shared_ptr<Map>> _mapLayers;

	void _generate(MapLayer layer);
	void _generateEnemies(MapLayer layer);
	int _toIndex(const clan::Vec2i &pos, int width) const { return pos.x + pos.y * width; }
};