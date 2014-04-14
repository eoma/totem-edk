
#pragma once

#include "ui_component.h"

class UIModelScene : public UIComponent
{
public:
	UIModelScene(clan::GUIComponent *parent);
	UIModelScene(UIScreen *screen);

protected:
	void init();
	void on_render(clan::Canvas &canvas, const clan::Rect &update_box);
};
