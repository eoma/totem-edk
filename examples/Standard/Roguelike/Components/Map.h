#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"
#include "../Utils/Vec2i.h"
#include "../Utils/Persistent.h"

#include <libtcod.hpp>
#include <memory>

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 10;
static const int MAX_ROOM_ITEMS = 5;

// after 20 turns, the monster cannot smell the scent anymore
static const int SCENT_THRESHOLD=20;

struct Tile {
    bool explored; // has the player already seen this tile ?
	unsigned int scent; // amount of player scent on this cell
    Tile() : explored(false), scent(0) {}
};

class RenderSystem;
typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class Map;
typedef std::shared_ptr<Map> MapPtr;

class Map : public Totem::Component<Map, PropertyUserData>, public Persistent
{
public:
	Map(const EntityWPtr &owner, int width, int height, const RenderSystemPtr &system);
    virtual ~Map();

	void init(bool withEntities);

	EntityPtr getOwner() { return owner.lock(); }

	bool isInFov(const Vec2i &pos) const;
	bool isWall(const Vec2i &pos) const;
    bool isExplored(const Vec2i &pos) const;
    void computeFov();

	bool canWalk(const Vec2i &pos) const;

	void render() const;

	void addMonster(const Vec2i &pos);
	void addItem(const Vec2i &pos);

	unsigned int currentScentValue;
	unsigned int getScent(const Vec2i &pos) const;

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	//Defined in Persistent.cpp
	void save(TCODZip &zip) override;
	void load(TCODZip &zip) override;

protected:
	Tile *tiles;
	TCODMap *map;
	friend class BspListener;
	void dig(const Vec2i &pos1, const Vec2i &pos2);
    void createRoom(bool first, const Vec2i &pos1, const Vec2i &pos2, bool withEntities);

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	RenderSystemPtr system;
	
	int width,height;

	TCODRandom *rng;
	long seed;
};