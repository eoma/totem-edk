
#pragma once

#include "Totem/ComponentContainer.h"
#include "Totem/PropertyContainer.h"

class Entity : public Totem::ComponentContainer<>, public Totem::PropertyContainer<>
{
public:
	Entity();

	void update(float time_elapsed);
};
