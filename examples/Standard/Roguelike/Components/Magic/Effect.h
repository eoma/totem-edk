#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../../Entity.h"
#include "../../Utils/TargetSelector.h"

#include <memory>

#include <libtcod.hpp>

class TargetSelector; typedef std::shared_ptr<TargetSelector> TargetSelectorPtr;

class Effect;
typedef std::shared_ptr<Effect> EffectPtr;

class Effect : public Totem::Component<Effect, PropertyUserData>
{
public:
	Effect(const EntityWPtr &owner);
    virtual ~Effect();

	EntityPtr getOwner() { return owner.lock(); }

	TargetSelectorPtr setSelector(TargetSelectorPtr selector) { this->selector = selector; return selector; }
	TargetSelectorPtr getSelector() const { return selector; }

	void use(EntityPtr wearer);

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	TargetSelectorPtr selector;

	void applyTo(EntityPtr target);
};