#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"

#include <memory>

#include <libtcod.hpp>

class RenderSystem;
typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class Destructible;
typedef std::shared_ptr<Destructible> DestructiblePtr;

class Destructible : public Totem::Component<Destructible, PropertyUserData>
{
public:
	Destructible(const EntityWPtr &owner, const RenderSystemPtr &system);
    virtual ~Destructible();

	EntityPtr getOwner() { return owner.lock(); }

	inline bool isDead() { return hp <= 0; }

	void takeDamage(float damage);
	void die();
	void heal(float amount);

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	RenderSystemPtr system;

	Totem::Property<std::string> corpseName; // the owner's name once dead/destroyed

	//Shared properties
	Totem::Property<int> ch;
	Totem::Property<TCODColor> col;
	Totem::Property<bool> blocks;

	Totem::Property<bool> dead;
	Totem::Property<float> defense; // hit points deflected

	Totem::Property<float> maxHp; // maximum health points
	Totem::Property<float> hp; // current health points
};