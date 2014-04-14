
#include "precomp.h"
#include "component_visual.h"
#include "visual_manager.h"

Visual::Visual(Entity *owner, VisualManager &visual_manager, const CL_String &name)
: Component<Visual>(name)
{
	property_sprite = owner->add<CL_Sprite>("sprite", CL_Sprite());
	property_color = owner->add<CL_Colorf>("color", CL_Colorf(1.0f,1.0f,1.0f,1.0f));
	property_position = owner->add<CL_Pointf>("position", CL_Pointf());
	property_scale = owner->add<CL_Pointf>("scale", CL_Pointf(1.0f,1.0f));
	property_alignment = owner->add<CL_Origin>("alignment", origin_center);

	slot = property_sprite.valueChanged().connect(this, &Visual::on_sprite_changed);
	slot = property_color.valueChanged().connect(this, &Visual::on_color_changed);

	visual_manager.add_visual(this);
}

void Visual::render(CL_GraphicContext &gc)
{
	CL_Sprite sprite = property_sprite;
	if(!sprite.is_null())
	{
		sprite.set_alignment(property_alignment.get());
		sprite.set_scale(property_scale.get().x, property_scale.get().y);
		sprite.draw(gc, property_position.get().x, property_position.get().y);
	}
}

void Visual::on_sprite_changed(const CL_Sprite &old_value, const CL_Sprite &new_value)
{
	if(new_value.is_null())
		return;

	property_sprite.get().set_color(property_color.get());
}

void Visual::on_color_changed(const CL_Colorf &old_value, const CL_Colorf &new_value)
{
	if(property_sprite.get().is_null())
		return;

	property_sprite.get().set_color(new_value);
}
