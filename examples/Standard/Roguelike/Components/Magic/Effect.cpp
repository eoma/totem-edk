#include "Effect.h"
#include "../../Engine.h"
#include "../Actor.h"
#include "../Container.h"

#include <iostream>

Effect::Effect(const EntityWPtr &owner) 
: Totem::Component<Effect, PropertyUserData>("Effect"), owner(owner), selector(nullptr)
{
	user_data.entity = owner;
	user_data.component = this;

	owner.lock()->registerToEvent1<EntityPtr>("Use").connect(this, &Effect::use);
}

Effect::~Effect()
{
	//std::cout << "Effect is being destroyed!" << std::endl;
}

void Effect::use(EntityPtr wearer)
{
	// Requires that the wearer is an actor for now...
	if( !wearer->hasComponent<Actor>() )
		return;

	if(selector)
	{
		auto targets = selector->selectTargets(wearer->getComponent<Actor>());
		for(unsigned int i = 0; i < targets.size(); i++)
		{
			auto target = targets[i];
			applyTo(target->getOwner());
		}
	}
	else
	{
		applyTo(wearer);
	}
}

void Effect::applyTo(EntityPtr target) 
{
	owner.lock()->sendEvent1<EntityPtr>("ApplyEffect", target);
}

