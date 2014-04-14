#include "Health.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Definitions.h"
#include <iostream>

Health::Health(Entity &owner, EntityManager &entity_manager) 
	: Totem::Component<Health>(get_type()), owner(owner), entity_manager(entity_manager), alive(true)
{
	health_property = owner.add<int>(PROPERTY_HEALTH, 100);
	health_property.valueChanged().connect(this, &Health::on_health_changed);

	owner.registerToEvent<int>(EVENT_TAKE_DAMAGE).connect(this, &Health::on_take_damage);
}

void Health::on_health_changed(const int &old_value, const int &new_value) 
{ 
	std::cout << "Health of " << owner.get_name() << " changed from " << old_value << " to " << new_value << std::endl; 
	if(alive && new_value <= 0)
	{
		std::cout << owner.get_name() << " died!" << std::endl;
		alive = false;

		owner.sendEvent<>(EVENT_DIED, false);
		entity_manager.remove_entity(owner.get_name());
	}
}

void Health::on_take_damage(int damage)
{
	health_property -= damage;
}
