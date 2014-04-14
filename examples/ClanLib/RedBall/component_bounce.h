
#pragma once

#include "entity.h"
#include "Totem/Component.h"

class Bounce : public Totem::Component<Bounce>
{
public:
	Bounce(Entity *owner, const CL_String &name = CL_String());

	virtual void update(const float &delta_time);

protected:
	Totem::Property<float> property_bounce_height;
	Totem::Property<float> property_bounce_speed;
	Totem::Property<float> property_bounce_distance;
	Totem::Property<CL_Pointf> property_position;
	Totem::Property<CL_String> property_sound;

	void on_position_changed(const CL_Pointf &old_value, const CL_Pointf &new_value);

	float total_time;
	CL_Pointf original_position;

	float curve_sign;

	CL_Slot slot;
};
