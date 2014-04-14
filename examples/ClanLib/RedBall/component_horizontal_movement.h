
#pragma once

#include "entity.h"
#include "Totem/Component.h"

class HorizontalMovement : public Totem::Component<HorizontalMovement>
{
public:
	HorizontalMovement(Entity *owner, const CL_String &name = CL_String());

	virtual void update(const float &delta_time);

protected:
	Totem::Property<CL_Pointf> property_position;
	Totem::Property<float> property_horizontal_movement_speed;

	float direction;
};
