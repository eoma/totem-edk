
#pragma once

#include "ui_component.h"
#include "ui_label.h"
#include "scaled_sprite.h"

class UIButton : public UIComponent
{
public:
	UIButton(clan::GUIComponent *parent);
	UIButton(UIScreen *screen);

	void set_scaled_geometry(ScaledBox new_scaled_box);
	virtual void update_geometry();
	clan::Callback_v0 &func_clicked() { return cb_clicked; }

	UILabel label;
	ScaledSprite background;

private:
	void on_render(clan::Canvas &canvas, const clan::Rect &update_box);
	bool on_input_pressed(const clan::InputEvent &input_event);
	bool on_input_released(const clan::InputEvent &input_event);
	bool on_input_pointer_moved(const clan::InputEvent &input_event);

	clan::Callback_v0 cb_clicked;
};
