#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"

#include <memory>

#include <libtcod.hpp>

class Ai;
typedef std::shared_ptr<Ai> AiPtr;

class Ai : public Totem::Component<Ai, PropertyUserData>
{
public:
	Ai(const EntityWPtr &owner);
    virtual ~Ai();

	EntityPtr getOwner() { return owner.lock(); }

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	
	
};