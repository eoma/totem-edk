#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"

#include <memory>

#include <libtcod.hpp>

class Attacker;
typedef std::shared_ptr<Attacker> AttackerPtr;

class Attacker : public Totem::Component<Attacker, PropertyUserData>
{
public:
	Attacker(const EntityWPtr &owner);
    virtual ~Attacker();

	EntityPtr getOwner() { return owner.lock(); }

	void attack(EntityPtr target);

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	
	Totem::Property<float> power;

	Totem::Property<float> maxHp; // maximum health points
	Totem::Property<float> hp; // current health points
};