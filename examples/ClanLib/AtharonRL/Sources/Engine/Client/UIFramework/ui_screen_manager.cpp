
#include "precomp.h"
#include "ui_screen_manager.h"
#include "mouse_movement.h"
#include "window_manager.h"
#include <algorithm>

using namespace clan;

UIScreenManager::UIScreenManager(const DisplayWindowDescription &desc, const std::string &large_icon, const std::string &small_icon)
: current(0)
{
	window = DisplayWindow(desc);
	
	window.set_large_icon(ImageProviderFactory::load(large_icon));
	window.set_small_icon(ImageProviderFactory::load(small_icon));
}

UIScreenManager::~UIScreenManager()
{
	while (!screens.empty())
		delete screens.back();
}

void UIScreenManager::hide_cursor()
{
	window.hide_cursor();
}

void UIScreenManager::maximize()
{
	window.maximize();
}

void UIScreenManager::add_screen(UIScreen *screen)
{
	screens.push_back(screen);
}

void UIScreenManager::remove_screen(UIScreen *screen)
{
	screens.erase(std::find(screens.begin(), screens.end(), screen));
}

void UIScreenManager::set_active(UIScreen *screen)
{
	if (screen != current)
	{
		if (current)
			current->on_deactivated();
		current = screen;
		if (current)
			current->on_activated();
	}
}

bool UIScreenManager::is_active(const UIScreen *screen) const
{
	return current == screen;
}

void UIScreenManager::update()
{
	ScopeTimerResults::start();
	if (current)
	{
		bool we_really_do_have_focus = window.has_focus();
		if (window.get_hwnd() != ::GetForegroundWindow())
			we_really_do_have_focus = false;

		Point delta;
		if (we_really_do_have_focus)
		{
			delta = mouse_movement.pos() - mouse_movement_pos;
			mouse_movement_pos += delta;
		}
		else
		{
			mouse_movement_pos = mouse_movement.pos();
		}
		current->on_mouse_movement(delta.x, delta.y);
		if (!current->get_wm_provider()->is_mouse_enabled())
			reset_mouse_position();
		current->update();
	}

	window.flip(0);
	ScopeTimerResults::end();
}

void UIScreenManager::reset_mouse_position()
{
	bool we_really_do_have_focus = window.has_focus();
	if (window.get_hwnd() != ::GetForegroundWindow())
		we_really_do_have_focus = false;

	if (we_really_do_have_focus)
	{
		Point client_pos(window.get_viewport().get_center());
		Point screen_pos = window.client_to_screen(client_pos);
		SetCursorPos(screen_pos.x, screen_pos.y);
	}
	//MouseMovement::has_focus = we_really_do_have_focus;
}
