
#pragma once

#include "ui_screen.h"
#include "mouse_movement.h"

class UIScreenManager
{
public:
//	UIScreenManager();
	UIScreenManager(const clan::DisplayWindowDescription &desc, const std::string &large_icon, const std::string &small_icon);
	~UIScreenManager();

	clan::DisplayWindow get_window() { return window; }
	MouseMovement &get_mouse_movement() { return mouse_movement; }

	void hide_cursor();
	void maximize();

	void set_active(UIScreen *screen);
	bool is_active(const UIScreen *screen) const;
	void update();

private:
	void add_screen(UIScreen *screen);
	void remove_screen(UIScreen *screen);
	void reset_mouse_position();

	clan::DisplayWindow window;
	MouseMovement mouse_movement;
	clan::Point mouse_movement_pos;
	UIScreen *current;
	std::vector<UIScreen *> screens;

	friend class UIScreen;
};
