#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../../Entity.h"

#include <memory>

#include <libtcod.hpp>

class Weave;
typedef std::shared_ptr<Weave> WeavePtr;

class Weave : public Totem::Component<Weave, PropertyUserData>
{
public:
	Weave(const EntityWPtr &owner);
    virtual ~Weave();

	EntityPtr getOwner() { return owner.lock(); }

    void use(EntityPtr wearer);

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	
	
};