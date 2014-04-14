#include "Transform.h"
#include "Entity.h"
#include "Vec2i.h"
#include "Definitions.h"
#include <iostream>

Transform::Transform(Entity &owner) 
	: Totem::Component<Transform>(get_type()), owner(owner)
{
	position_property = owner.add<Vec2i>(PROPERTY_POSITION, Vec2i());
	position_property.valueChanged().connect(this, &Transform::on_position_changed);
}

void Transform::on_position_changed(const Vec2i &old_value, const Vec2i &new_value) 
{ 
	std::cout << "Position of " << owner.get_name() << " changed from " << old_value.x << ", " << old_value.y << " to " << new_value.x << ", " << new_value.y << std::endl; 
}