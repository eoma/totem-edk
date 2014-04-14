#include "Entity.h"
#include <iostream>

Entity::Entity(const std::string &name) 
	: name(name) 
{
	propertyAdded().connect(this, &Entity::on_property_added);
	propertyRemoved().connect(this, &Entity::on_property_removed);
	componentAdded().connect(this, &Entity::on_component_added);
	componentRemoved().connect(this, &Entity::on_component_removed);

	std::cout << name << " was created" << std::endl;
}

void Entity::on_property_added(std::shared_ptr<Totem::IProperty> property) 
{ 
	std::cout << property->getName() << " property was added to " << name << std::endl; 
}

void Entity::on_property_removed(std::shared_ptr<Totem::IProperty> property) 
{ 
	std::cout << property->getName() << " property was removed from " << name << std::endl; 
}

void Entity::on_component_added(std::shared_ptr<Totem::IComponent<>> component) 
{ 
	std::cout << component->getName() << " component was added to " << name << std::endl; 
}

void Entity::on_component_removed(std::shared_ptr<Totem::IComponent<>> component) 
{ 
	std::cout << component->getName() << " component was removed from " << name << std::endl; 
}