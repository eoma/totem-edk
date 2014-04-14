#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"

#include <memory>

#include <libtcod.hpp>

class Consumable;
typedef std::shared_ptr<Consumable> ConsumablePtr;

class Consumable : public Totem::Component<Consumable, PropertyUserData>
{
public:
	Consumable(const EntityWPtr &owner);
    virtual ~Consumable();

	EntityPtr getOwner() { return owner.lock(); }

    void use(EntityPtr wearer);

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	
	
};