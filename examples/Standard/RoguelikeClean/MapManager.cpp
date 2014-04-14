#include "MapManager.h"
#include "GameManager.h"
#include "Player.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "EntityDefinitions.h"
#include "PropertyDefinitions.h"

using namespace clan;

MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}

void MapManager::initialize(const Parser::MapsInfo &mapsInfo)
{
	_mapLayers.resize(MAP_LAYER_COUNT, nullptr);

	for(auto info : mapsInfo.Maps)
	{
		if(info->Layer < 0 || info->Layer >= MAP_LAYER_COUNT)
			continue;

		auto layer = std::make_shared<Map>((MapLayer)info->Layer, Vec2i(info->Width, info->Height));
		layer->generationType = (info->GenerationType >= 0 && info->GenerationType < GENERATION_TYPE_COUNT) ? (GenerationType)(info->GenerationType) : GENERATION_NONE;
		layer->groundInViewColor = info->GroundInViewColor;
		layer->wallInViewColor = info->WallInViewColor;
		layer->groundInMemoryColor = info->GroundInMemoryColor;
		layer->wallInMemoryColor = info->WallInMemoryColor;
		_mapLayers[info->Layer] = layer;
	}
}

void MapManager::generate()
{
	_generate(LAYER_GROUND);
	_generate(LAYER_AIR);
}

void MapManager::_generate(MapLayer layer)
{
	auto map = _mapLayers[layer];
	switch(map->generationType)
	{
	case GENERATION_BSP:
		{
			TCODBsp bsp(0,0, map->size.x, map->size.y);
			bsp.splitRecursive(nullptr, 8, 12, 6, 1.5f, 1.5f);
			MapBspListener listener(map->layer);
			bsp.traverseInvertedLevelOrder(&listener, nullptr);
		}
		break;
	case GENERATION_NONE:
	default:
		break;
	};
}

void MapManager::generateEnemies()
{
	_generateEnemies(LAYER_GROUND);
	_generateEnemies(LAYER_AIR);
}

void MapManager::_generateEnemies(MapLayer layer)
{
	auto rng = TCODRandom::getInstance();
	auto map = _mapLayers[layer];
	for(auto position : map->enemyPositions)
	{
		std::shared_ptr<Entity> enemy;
		auto value = rng->getInt(0,100);
		if(value < 90)
			enemy = GameManager::Get()->getFactory()->instantiate(ENTITY_GOBLIN);
		else
			enemy = GameManager::Get()->getFactory()->instantiate(ENTITY_TROLL);

		if(enemy && enemy->hasProperty(PROPERTY_POSITION))
			enemy->get<Vec2i>(PROPERTY_POSITION) = position;
	}
}

void MapManager::render(const std::shared_ptr<TCODConsole> &canvas, MapLayer layer)
{
	auto player = GameManager::Get()->getPlayer()->Get();
	auto position = player->get<Vec2i>(PROPERTY_POSITION).get();
	auto radius = player->get<int>(PROPERTY_SIGHT_RADIUS).get();

	auto map = _mapLayers[layer];
	for(int x = 0; x < map->size.x; x++)
	{
		for(int y = 0; y < map->size.y; y++)
		{
			auto cell = Vec2i(x,y);
			if(isInFov(layer, cell))
			{
				auto distance = position.distance(cell);
				auto fadeMod = min(1.0f - (distance / (float)radius) + 0.75f - ((rand()%2+1)*0.025f), 1.0f);
				fadeMod *= fadeMod;
				canvas->setCharBackground(x,y, (isWall(layer, cell) ? map->wallInViewColor : map->groundInViewColor) * fadeMod);
			}
			else if(isExplored(layer, cell))
			{
				canvas->setCharBackground(x,y, isWall(layer, cell) ? map->wallInMemoryColor : map->groundInMemoryColor);
			}
		}
	}
}

bool MapManager::isInFov(MapLayer layer, const clan::Vec2i &pos) const
{
	if(_mapLayers.empty())
		return false;

	auto map = _mapLayers[layer];
	if(map->data->isInFov(pos.x, pos.y))
	{
		map->tiles[ _toIndex(pos,map->size.x) ]->explored = true;
		return true;
	}

	return false;
}

bool MapManager::isWall(MapLayer layer, const clan::Vec2i &pos) const
{
	return !_mapLayers[layer]->data->isWalkable(pos.x, pos.y);
}

bool MapManager::isExplored(MapLayer layer, const clan::Vec2i &pos) const
{
	auto map = _mapLayers[layer];
	return map->tiles[ _toIndex(pos,map->size.x) ]->explored;
}

void MapManager::computeFov(MapLayer layer, const clan::Vec2i &pos, int radius)
{
	_mapLayers[layer]->data->computeFov(pos.x, pos.y, radius, true, FOV_SHADOW);
}

void MapManager::dig(MapLayer layer, clan::Vec2i bl, clan::Vec2i tr)
{
	if(tr.x < bl.x)
	{
		int tmp = tr.x;
		tr.x = bl.x;
		bl.x = tmp;
	}

	if(tr.y < bl.y)
	{
		int tmp = tr.y;
		tr.y = bl.y;
		bl.y = tmp;
	}

	for(int x = bl.x; x <= tr.x; x++)
	{
		for(int y = bl.y; y <= tr.y; y++)
		{
			_mapLayers[layer]->data->setProperties(x,y, true,true);
		}
	}
}

void MapManager::createRoom(MapLayer layer, bool first, const clan::Vec2i &bl, const clan::Vec2i &tr)
{
	dig(layer, bl,tr);
	
	if(first)
	{
		auto pawn = GameManager::Get()->getPlayer()->Get();
		if(pawn && pawn->hasProperty(PROPERTY_POSITION))
			pawn->get<Vec2i>(PROPERTY_POSITION) = (bl+tr)/2;
			
	}
	else
	{
		auto rng = TCODRandom::getInstance();
		if(rng->getInt(0,3) == 0)
		{
			auto monsterCount = rng->getInt(0,3);
			while(monsterCount > 0)
			{
				auto position = Vec2i(rng->getInt(bl.x,tr.x), rng->getInt(bl.y,tr.y));
			
				if(isWall(layer, position) == false)
					_mapLayers[layer]->enemyPositions.push_back(position);

				monsterCount--;
			}
		}
	}
}

bool MapManager::MapBspListener::visitNode(TCODBsp *node, void *userData)
{
	if(node->isLeaf())
	{
		Vec2i pos;
		Vec2i size;

		auto rng = TCODRandom::getInstance();

		size = Vec2i(
			rng->getInt(6, node->w-2), 
			rng->getInt(6, node->h-2));

		pos  = Vec2i(
			rng->getInt(node->x+1, node->x + node->w - size.x - 1), 
			rng->getInt(node->y+1, node->y + node->h - size.y - 1));

		auto map = GameManager::Get()->getMap();
		map->createRoom(_layer, _roomNum==0, pos, pos+size-1);

		if(_roomNum != 0)
		{
			map->dig(_layer, _last, Vec2i(pos.x+size.x/2, _last.y));
			map->dig(_layer, Vec2i(pos.x+size.x/2, _last.y), Vec2i(pos.x+size.x/2, pos.y+size.y/2));
		}

		_last = pos+size/2;
		_roomNum++;
	}
	return true;
}
