
#include "precomp.h"
#include "component_horizontal_movement.h"

HorizontalMovement::HorizontalMovement(Entity *owner, const CL_String &name)
: Component<HorizontalMovement>(name)
{
	property_position = owner->add<CL_Pointf>("position", CL_Pointf());
	property_horizontal_movement_speed = owner->add<float>("horizontal_movement_speed", 25.0f);
	direction = 1.0f;
}

void HorizontalMovement::update(const float &delta_time)
{
	CL_Pointf position = property_position;

	position.x = position.x + (delta_time * property_horizontal_movement_speed / 100.0f * direction);
	if(position.x <= 0)
		direction = 1.0f;
	if(position.x >= 800)
		direction = -1.0f;

	property_position.set(position, false);
}
