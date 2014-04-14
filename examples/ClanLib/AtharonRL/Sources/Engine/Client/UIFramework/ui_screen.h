
#pragma once

class WindowManagerProvider;
class UIScreenManager;
class UIScreen_Impl;

class UIScreen
{
public:
	UIScreen(UIScreenManager *manager);
	virtual ~UIScreen();

	UIScreenManager *get_manager();
	clan::GUIManager *get_gui();
	WindowManagerProvider *get_wm_provider();

	clan::GraphicContext get_gc();
	clan::Canvas &get_canvas();

	bool is_game_input_enabled();
	clan::InputContext get_ic();

	clan::Point get_mouse_movement_pos();
	bool get_keycode(int code);

	void set_active();
	bool is_active() const;
	void enable_mouse(bool enable);
	virtual void update();

	virtual void on_activated();
	virtual void on_deactivated();
	void on_mouse_movement(int delta_x, int delta_y);

	clan::Signal_v1<const clan::InputEvent &> sig_mouse_input;

private:
	UIScreen(const UIScreen &other); // do not implement
	UIScreen &operator=(const UIScreen &other); // do not implement
	void on_mouse_input_helper(clan::InputEvent e);

	UIScreenManager *manager;
	clan::GUIWindowManager wm;
	clan::GUIManager gui;
	clan::Point mouse_movement_pos;
	clan::SlotContainer slots;
};
