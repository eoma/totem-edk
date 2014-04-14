#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../../Entity.h"

#include <memory>

#include <libtcod.hpp>

class Healer;
typedef std::shared_ptr<Healer> HealerPtr;

class Healer : public Totem::Component<Healer, PropertyUserData>
{
public:
	Healer(const EntityWPtr &owner);
    virtual ~Healer();

	EntityPtr getOwner() { return owner.lock(); }

    void applyEffect(EntityPtr victim);

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	
	Totem::Property<float> amount; // how many hp
	Totem::Property<std::string> message;
};