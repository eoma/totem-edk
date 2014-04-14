#include "Map.h"
#include "Actor.h"
#include "../Systems/PrefabSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/MonsterSystem.h"
#include "../Engine.h"
#include "../Utils/BspListener.h"
#include "../Utils/TargetSelector.h"

#include "Destructible.h"
#include "Attacker.h"
#include "Monster.h"
#include "Pickable.h"
#include "Consumable.h"
#include "Magic/Effect.h"
#include "Magic/Healer.h"
#include "Magic/Weave.h"

#include <iostream>

Map::Map(const EntityWPtr &owner, int width, int height, const RenderSystemPtr &system) 
:	Totem::Component<Map, PropertyUserData>("Map"), owner(owner), width(width), height(height), 
system(system), currentScentValue(SCENT_THRESHOLD), tiles(nullptr), map(nullptr)
{
	user_data.entity = owner;
	user_data.component = this;

	owner.lock()->registerToEvent0("ComputeFOV").connect(this, &Map::computeFov);

	system->set(this);

	seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
}

Map::~Map()
{
	//std::cout << "Map is being destroyed!" << std::endl;
	delete [] tiles;
	delete map;
	system->set(static_cast<Map*>(nullptr));
}

void Map::init(bool withEntities)
{
	if(tiles)	delete[] tiles;
	if(map)		delete map;

	rng = new TCODRandom(seed, TCOD_RNG_CMWC);

	tiles=new Tile[width*height];
	map=new TCODMap(width,height);

	TCODBsp bsp(0,0,width,height);
    bsp.splitRecursive(rng,8,ROOM_MIN_SIZE,ROOM_MAX_SIZE,1.75f,1.75f);
    BspListener listener(this);
    bsp.traverseInvertedLevelOrder(&listener,(void*)withEntities);
}

void Map::render() const
{ 
	static const TCODColor darkWall(0,0,100);	
	static const TCODColor darkGround(50,50,150);
	static const TCODColor lightWall(130,110,50);
	static const TCODColor lightGround(200,180,50);

	for (int x=0; x < width; x++) {
    for (int y=0; y < height; y++){

		auto pos = Vec2i(x,y);
		int scent=SCENT_THRESHOLD - (currentScentValue - getScent(pos));
        scent = CLAMP(0,10,scent);
        float sc=scent * 0.1f;

        if ( isInFov(pos) )
            TCODConsole::root->setCharBackground(x,y, isWall(pos) ? lightWall :lightGround * sc );
        else if ( isExplored(pos) )
            TCODConsole::root->setCharBackground(x,y, isWall(pos) ? darkWall : darkGround * sc);
		else if ( !isWall(pos) )
			TCODConsole::root->setCharBackground(x,y, TCODColor::white * sc);
    }}
}

unsigned int Map::getScent(const Vec2i &pos) const {
    return tiles[pos.x()+pos.y()*width].scent;
}

bool Map::isWall(const Vec2i &pos) const {
    return !map->isWalkable(pos.x(), pos.y());
}

bool Map::isExplored(const Vec2i &pos) const {
    return tiles[pos.x()+pos.y()*width].explored;
}

bool Map::isInFov(const Vec2i &pos) const {
	if ( pos.x() < 0 || pos.x() >= width || pos.y() < 0 || pos.y() >= height ) {
        return false;
    }
    if ( map->isInFov(pos.x(), pos.y()) ) {
        tiles[pos.x()+pos.y()*width].explored=true;
        return true;
    }
    return false;
}
 
void Map::computeFov() {
	auto engine = Engine::getSingleton();
	auto player = engine->getPlayer();
	map->computeFov(player->x(),player->y(),
        engine->fovRadius);

	if(player->isDead())
		return;

	// update scent field
    for (int x=0; x < width; x++) 
	{
        for (int y=0; y < height; y++) 
		{
			auto pos = Vec2i(x,y);
            if (isInFov(pos)) 
			{
                unsigned int oldScent=getScent(pos);
				auto distance = pos.distance(player->getPosition());
                unsigned int newScent=currentScentValue-distance;
                if (newScent > oldScent) {
                    tiles[x+y*width].scent = newScent;
                }
            }
        }
    }
}

bool Map::canWalk(const Vec2i &pos) const
{
    if (isWall(pos)) {
        // this is a wall
        return false;
    }
	auto actors = Engine::getSingleton()->getActors();
    for (unsigned int i = 0; i < actors.size(); i++) {
        auto actor = actors[i];
        if ( actor->blocksTile() && actor->getPosition() == pos ) {
            // there is a blocking actor there. cannot walk
            return false;
        }
    }
    return true;
}

void Map::dig(const Vec2i &pos1, const Vec2i &pos2) 
{
	int x1 = pos1.x();
	int x2 = pos2.x();
	int y1 = pos1.y();
	int y2 = pos2.y();

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
    for (int tilex=x1; tilex <= x2; tilex++) {
    for (int tiley=y1; tiley <= y2; tiley++) {
        map->setProperties(tilex,tiley,true,true);
    }
	}
}

void Map::createRoom(bool first, const Vec2i &pos1, const Vec2i &pos2, bool withEntities)
{
	auto engine = Engine::getSingleton();

	int x1 = pos1.x();
	int x2 = pos2.x();
	int y1 = pos1.y();
	int y2 = pos2.y();

    dig (pos1,pos2);
	
	if(!withEntities)
		return;

    if ( first ) 
	{
        // put the player in the first room
        engine->getPlayer()->getOwner()->get<Vec2i>("Position") = Vec2i( (x1+x2)/2, (y1+y2)/2 );
    } 
	else 
	{
		// add monsters
		int nbMonsters = rng->getInt(0,MAX_ROOM_MONSTERS);
		while (nbMonsters > 0) 
		{
			auto rngPos = Vec2i(rng->getInt(x1,x2), rng->getInt(y1,y2));
			if ( canWalk(rngPos) )
				addMonster(rngPos);
			nbMonsters--;
		}

		// add items
		int nbItems = rng->getInt(0,MAX_ROOM_ITEMS);
		while (nbItems > 0) 
		{
			auto rngPos = Vec2i(rng->getInt(x1,x2), rng->getInt(y1,y2));
			if ( canWalk(rngPos) )
				addItem(rngPos);
			nbItems--;
		}
    }
}

void Map::addMonster(const Vec2i &pos) {
	auto engine = Engine::getSingleton();
	auto prefab_system = engine->getPrefabSystem();
	auto monster_system = engine->getMonsterSystem();
	EntityPtr monster = nullptr;

	auto value = rng->getInt(0,1000);
    if ( value < 800 )
		monster = monster_system->instantiate(GOBLIN_FAMILY, 1);
	else if(value < 990)
		monster = monster_system->instantiate(GOBLIN_FAMILY, 2);
	else
		monster = monster_system->instantiate(GOBLIN_FAMILY, 3);

	if(monster)
		monster->get<Vec2i>("Position") = pos;
}

void Map::addItem(const Vec2i &pos) {
	auto engine = Engine::getSingleton();
	auto prefab_system = engine->getPrefabSystem();

	EntityPtr item = nullptr;

	auto dice = rng->getInt(0,100);
	if ( dice < 50 ) 
	{
		item = prefab_system->instantiate("health potion");
		/*auto potion = engine->createActor("health potion", pos, '!', TCODColor::violet);
		potion->addComponent( std::make_shared<Pickable>(potion, system) );
		potion->addComponent( std::make_shared<Consumable>(potion) );
		potion->addComponent( std::make_shared<Effect>(potion) );
		potion->addComponent( std::make_shared<Healer>(potion) );
		potion->get<bool>("Blocks") = false;
		potion->get<float>("Amount") = 30.0f;*/
	}
	else if( dice < 75 )
	{
		item = prefab_system->instantiate("scroll of lightning bolt");
		/*auto scroll = engine->createActor("scroll of lightning bolt", pos, '#', TCODColor::lightYellow);
		scroll->addComponent( std::make_shared<Pickable>(scroll, system) );
		scroll->addComponent( std::make_shared<Weave>(scroll) );
		auto effect = scroll->addComponent( std::make_shared<Effect>(scroll) );
		scroll->addComponent( std::make_shared<Healer>(scroll) );
		scroll->get<bool>("Blocks") = false;
		scroll->get<float>("Amount") = -20.0f;
		scroll->get<std::string>("Message") = "A lighting bolt strikes the %s with a loud thunder!\nThe damage is %g hit points.";
		effect->setSelector(std::make_shared<TargetSelector>(TargetSelector::CLOSEST_MONSTER))->setRange(5.0f);*/
	}
	else
	{
		item = prefab_system->instantiate("scroll of fireball");
		/*auto scroll = engine->createActor("scroll of fireball", pos, '#', TCODColor::lightYellow);
		scroll->addComponent( std::make_shared<Pickable>(scroll, system) );
		scroll->addComponent( std::make_shared<Weave>(scroll) );
		auto effect = scroll->addComponent( std::make_shared<Effect>(scroll) );
		scroll->addComponent( std::make_shared<Healer>(scroll) );
		scroll->get<bool>("Blocks") = false;
		scroll->get<float>("Amount") = -12.0f;
		scroll->get<std::string>("Message") = "The %s gets burned for %g hit points.";
		effect->setSelector(std::make_shared<TargetSelector>(TargetSelector::SELECTED_RANGE))->setRange(3.0f);*/
	}

	if(item && item->hasProperty("Position"))
		item->get<Vec2i>("Position") = pos;
}