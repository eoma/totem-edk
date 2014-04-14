
#pragma once

#include "scaled_box.h"
#include "Engine/Client/UIFramework/ui_screen.h"

class UIScreen;
class WindowManagerProvider;

class UIComponent : public clan::GUIComponent
{
public:
	UIComponent(clan::GUIComponent *parent);
	UIComponent(UIScreen *screen);

	void set_scaled_geometry(ScaledBox scaled_box);
	virtual void update_geometry();

	float get_scale();
	WindowManagerProvider *get_wm_provider();

private:
	static clan::GUITopLevelDescription get_toplevel_description();

	ScaledBox scaled_box;
};
