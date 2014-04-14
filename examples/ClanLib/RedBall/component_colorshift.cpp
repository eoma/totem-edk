
#include "precomp.h"
#include "component_colorshift.h"

ColorShift::ColorShift(Entity *owner, const CL_String &name)
: Component<ColorShift>(name)
{
	property_sprite = owner->add<CL_Sprite>("sprite", CL_Sprite());
	property_color = owner->add<CL_Colorf>("color", CL_Colorf(1.0f,0.1f,0.1f,1.0f));
	property_color_to = owner->add<CL_Colorf>("color_to", CL_Colorf(0.1f,0.1f,1.0f, 1.0f));
	property_shift_speed = owner->add<float>("shift_speed", 100.0f);

	total_time = 0.0f;
}

void ColorShift::update(const float &delta_time)
{
	total_time += delta_time*0.01f;

	float curve = ((sinf(total_time * (property_shift_speed / 1000)+(CL_M_PI/4))) + 1.0f) / 2.0f;  
	CL_Colorf color;
	color.r = (property_color.get().r+(property_color_to.get().r-property_color.get().r)*curve);
	color.g = (property_color.get().g+(property_color_to.get().g-property_color.get().g)*curve);
	color.b = (property_color.get().b+(property_color_to.get().b-property_color.get().b)*curve);
	color.a = (property_color.get().a+(property_color_to.get().a-property_color.get().a)*curve);

	property_sprite.get().set_color(color);
	//property_sprite.get().set_alpha(color.a);
}

