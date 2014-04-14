#include "Explosive.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Vec2i.h"
#include "Definitions.h"
#include <iostream>

Explosive::Explosive(Entity &owner, EntityManager &entity_manager) 
	: Totem::Component<Explosive>(get_type()), owner(owner), entity_manager(entity_manager)
{
	explosion_radius_property = owner.add<int>(PROPERTY_EXPLOSION_RADIUS, 4);
	explosion_damage_property = owner.add<int>(PROPERTY_EXPLOSION_DAMAGE, 100);
	position_property = owner.add<Vec2i>(PROPERTY_POSITION, Vec2i());

	owner.registerToEvent<>(EVENT_DIED).connect(this, &Explosive::on_explode);
}

void Explosive::on_explode() 
{ 
	std::cout << owner.get_name() << " EXPLODES!" << std::endl; 
	for(auto entity : entity_manager.get_entities())
	{
		if(entity.get() == &owner)
			continue;

		if(entity->hasProperty(PROPERTY_POSITION) == false || entity->hasProperty(PROPERTY_HEALTH) == false)
			continue;

		if(entity->get<int>(PROPERTY_HEALTH) <= 0)
			continue;

		int distance = Vec2i::distance(position_property, entity->get<Vec2i>(PROPERTY_POSITION));
		std::cout << "Distance between " << owner.get_name() << " and " << entity->get_name() << " is " << distance << std::endl;
		if(explosion_radius_property >= distance)
		{
			std::cout << entity->get_name() << " is caught in the blast radius!" << std::endl; 
			entity->sendEvent<int>(EVENT_TAKE_DAMAGE, explosion_damage_property);
		}
		else
		{
			std::cout << entity->get_name() << " is outside the blast radius." << std::endl; 
		}
	}
}
