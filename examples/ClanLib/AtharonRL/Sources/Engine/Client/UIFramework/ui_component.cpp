
#include "precomp.h"
#include "ui_component.h"
#include "window_description.h"
#include "window_manager.h"

using namespace clan;

UIComponent::UIComponent(GUIComponent *parent)
: GUIComponent(parent, "uicomponent")
{
}

UIComponent::UIComponent(UIScreen *screen)
: GUIComponent(screen->get_gui(), get_toplevel_description(), "uicomponent")
{
}

void UIComponent::set_scaled_geometry(ScaledBox new_scaled_box)
{
	scaled_box = new_scaled_box;
	update_geometry();
}

float UIComponent::get_scale()
{
	WindowManagerProvider *wm_provider = static_cast<WindowManagerProvider*>(get_gui_manager().get_window_manager().get_provider());
	return wm_provider->get_scale();
}

void UIComponent::update_geometry()
{
	GUIComponent *parent = get_parent_component();
	Size parent_size;
	if (parent)
		parent_size = parent->get_size();
	else
		parent_size = get_display_window().get_viewport().get_size();

	if (parent)
		set_geometry(scaled_box.get_geometry(parent_size, get_scale()));
	else
		set_window_geometry(scaled_box.get_geometry(parent_size, get_scale()));

	GUIComponent *child = get_first_child();
	while (child)
	{
		UIComponent *ui_component = dynamic_cast<UIComponent*>(child);
		if (ui_component)
			ui_component->update_geometry();
		child = child->get_next_sibling();
	}
}

GUITopLevelDescription UIComponent::get_toplevel_description()
{
	WindowDescription desc;
	return desc;
}

WindowManagerProvider *UIComponent::get_wm_provider()
{
	return static_cast<WindowManagerProvider*>(get_gui_manager().get_window_manager().get_provider());
}
