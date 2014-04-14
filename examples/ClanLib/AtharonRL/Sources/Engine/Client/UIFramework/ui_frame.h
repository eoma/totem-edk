
#pragma once

#include "ui_component.h"
#include "scaled_sprite.h"

class UIFrame : public UIComponent
{
public:
	UIFrame(clan::GUIComponent *parent);
	UIFrame(UIScreen *screen);

	ScaledSprite background;

	virtual void update_geometry();

protected:
	void on_render(clan::Canvas &canvas, const clan::Rect &update_box);
};
