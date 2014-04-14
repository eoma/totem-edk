#pragma once

#include <Totem/Component.h>
#include "../Entity.h"

#include "ComponentDefinitions.h"

class Agent;

class Ai : public Totem::Component<Ai>
{
public:
	Ai(Entity *owner);
	virtual ~Ai();

	static std::string GetType() { return COMPONENT_AI; }
	Entity *Owner() const { return _owner; }

private:
	Entity *_owner;
	std::shared_ptr<Agent> _agent;
};
