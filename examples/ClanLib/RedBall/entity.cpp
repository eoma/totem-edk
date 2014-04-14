
#include "precomp.h"
#include "entity.h"

Entity::Entity()
	: Totem::ComponentContainer<>(), Totem::PropertyContainer<>()
{
}

void Entity::update(float time_elapsed)
{
	updateComponents(time_elapsed);
	updateProperties();
}
