
#include "precomp.h"
#include "ui_screen.h"
#include "ui_screen_manager.h"
#include "window_manager.h"

using namespace clan;

UIScreen::UIScreen(UIScreenManager *manager)
: manager(manager)
{
	wm = GUIWindowManager(new WindowManagerProvider(manager->get_window()));
	gui = GUIManager(wm, "Resources/Engine");
	gui.add_resources("Resources/Engine/GUIThemeAero/resources.xml");
	slots.connect(get_wm_provider()->sig_mouse_input(), this, &UIScreen::on_mouse_input_helper);
	manager->add_screen(this);
}

UIScreen::~UIScreen()
{
	manager->remove_screen(this);
}

void UIScreen::on_mouse_input_helper(InputEvent e)
{
	sig_mouse_input.invoke(e);
}

UIScreenManager *UIScreen::get_manager()
{
	return manager;
}

GUIManager *UIScreen::get_gui()
{
	return &gui;
}

void UIScreen::set_active()
{
	manager->set_active(this);
}

bool UIScreen::is_active() const
{
	return manager->is_active(this);
}

void UIScreen::enable_mouse(bool enable)
{
	static_cast<WindowManagerProvider*>(wm.get_provider())->enable_mouse(enable);
}

void UIScreen::update()
{
	static_cast<WindowManagerProvider*>(wm.get_provider())->set_scale(manager->get_window().get_viewport().get_height() / 1080.0f);
	gui.process_messages(0);
	gui.render_windows();
}

void UIScreen::on_activated()
{
	get_wm_provider()->enable(true);
}

void UIScreen::on_deactivated()
{
	get_wm_provider()->enable(false);
}

void UIScreen::on_mouse_movement(int delta_x, int delta_y)
{
	if (get_wm_provider()->is_enabled() && !get_wm_provider()->is_mouse_enabled())
	{
		mouse_movement_pos.x += delta_x;
		mouse_movement_pos.y += delta_y;
	}
}

GraphicContext UIScreen::get_gc()
{
	return manager->get_window().get_gc();
}

Canvas &UIScreen::get_canvas()
{
	return get_wm_provider()->get_canvas(0);
}

InputContext UIScreen::get_ic()
{
	return manager->get_window().get_ic();
}

bool UIScreen::is_game_input_enabled()
{
	return get_wm_provider()->is_enabled() && !get_wm_provider()->is_mouse_enabled();
}

Point UIScreen::get_mouse_movement_pos()
{
	return mouse_movement_pos;
}

bool UIScreen::get_keycode(int code)
{
	if (is_game_input_enabled())
		return get_ic().get_keyboard().get_keycode(code);
	else
		return false;
}

WindowManagerProvider *UIScreen::get_wm_provider()
{
	return static_cast<WindowManagerProvider*>(wm.get_provider());
}
