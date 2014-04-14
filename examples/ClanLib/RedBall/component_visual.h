
#pragma once

#include "entity.h"
#include "Totem/Component.h"

class VisualManager;

class Visual : public Totem::Component<Visual>
{
public:
	Visual(Entity *owner, VisualManager &visual_manager, const CL_String &name = CL_String());

	void render(CL_GraphicContext &gc);

protected:
	Totem::Property<CL_Sprite> property_sprite;
	Totem::Property<CL_Colorf> property_color;
	Totem::Property<CL_Pointf> property_position;
	Totem::Property<CL_Pointf> property_scale;
	Totem::Property<CL_Origin> property_alignment;

	void on_sprite_changed(const CL_Sprite &old_value, const CL_Sprite &new_value);
	void on_color_changed(const CL_Colorf &old_value, const CL_Colorf &new_value);

	CL_Slot slot;
};
