
#pragma once

#include "entity.h"
#include "Totem/Component.h"

class ColorShift : public Totem::Component<ColorShift>
{
public:
	ColorShift(Entity *owner, const CL_String &name = CL_String());

	virtual void update(const float &delta_time);

protected:
	Totem::Property<CL_Sprite> property_sprite;
	Totem::Property<CL_Colorf> property_color;
	Totem::Property<CL_Colorf> property_color_to;
	Totem::Property<float> property_shift_speed;

	float total_time;

	CL_Slot slot;
};
