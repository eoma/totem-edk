
#pragma once

class WindowManagerProvider : public clan::GUIWindowManagerProvider
{
public:
	WindowManagerProvider(clan::DisplayWindow window);
	~WindowManagerProvider();
	void set_scale(float scale);
	float get_scale() const;

	void enable(bool enable);
	bool is_enabled() const { return enabled; }

	void enable_mouse(bool enable);
	bool is_mouse_enabled() const { return mouse_enabled; }

	void activate(clan::GUITopLevelWindow *window);
	void deactivate();

	clan::Signal_v1<clan::InputEvent> &sig_mouse_input() { return _sig_mouse_input; }

	clan::GUIWindowManager::WindowManagerType get_window_manager_type() const;
	void destroy();
	void set_site(clan::GUIWindowManagerSite *site);
	void create_window(clan::GUITopLevelWindow *handle, clan::GUITopLevelWindow *owner, clan::GUIComponent *component, clan::GUITopLevelDescription description);
	void destroy_window(clan::GUITopLevelWindow *handle);
	void enable_window(clan::GUITopLevelWindow *handle, bool enable);
	bool has_focus(clan::GUITopLevelWindow *handle) const;
	void set_visible(clan::GUITopLevelWindow *handle, bool visible, bool activate_root_win);
	void set_geometry(clan::GUITopLevelWindow *handle, const clan::Rect &geometry, bool client_area);
	clan::Rect get_geometry(clan::GUITopLevelWindow *handle, bool client_area) const;
	clan::Point screen_to_window(clan::GUITopLevelWindow *handle, const clan::Point &screen_point, bool client_area) const;
	clan::Point window_to_screen(clan::GUITopLevelWindow *handle, const clan::Point &window_point, bool client_area) const;
	clan::Canvas& get_canvas(clan::GUITopLevelWindow *handle);
	clan::InputContext get_ic(clan::GUITopLevelWindow *handle) const;
	clan::Canvas begin_paint(clan::GUITopLevelWindow *handle, const clan::Rect &update_region);
	void set_cliprect(clan::GUITopLevelWindow *handle, clan::Canvas &canvas, const clan::Rect &rect);
	void reset_cliprect(clan::GUITopLevelWindow *handle, clan::Canvas &canvas);
	void push_cliprect(clan::GUITopLevelWindow *handle, clan::Canvas &canvas, const clan::Rect &rect);
	void pop_cliprect(clan::GUITopLevelWindow *handle, clan::Canvas &canvas);
	void end_paint(clan::Canvas &canvas, clan::GUITopLevelWindow *handle, const clan::Rect &update_region);
	void request_repaint(clan::GUITopLevelWindow *handle, const clan::Rect &update_region);
	void bring_to_front(clan::GUITopLevelWindow *handle);
	bool is_minimized(clan::GUITopLevelWindow *handle) const;
	bool is_maximized(clan::GUITopLevelWindow *handle) const;
	void capture_mouse(clan::GUITopLevelWindow *handle, bool state);
	clan::DisplayWindow get_display_window(clan::GUITopLevelWindow *handle) const;
	void set_cursor(clan::GUITopLevelWindow *handle, const clan::Cursor &cursor);
	void set_cursor(clan::GUITopLevelWindow *handle, enum clan::StandardCursor type);
	void update();
	void setup_painting();
	void complete_painting();

private:
	void on_input_received(const clan::InputEvent &e);
	void on_mouse_input_received(const clan::InputEvent &e);
	void on_displaywindow_resize(int, int);
	void update_geometry();
	clan::GUITopLevelWindow *get_window_at_point(const clan::Point &point);
	void invoke_input_received(clan::GUITopLevelWindow *window, const clan::InputEvent &input_event);

	struct TopLevelWindow
	{
		TopLevelWindow() : owner(0), component(), enabled(true) { }
		TopLevelWindow(clan::GUITopLevelWindow *owner, clan::GUIComponent *component, clan::GUITopLevelDescription description) : owner(owner), component(component), description(description), enabled(true) { }

		clan::GUITopLevelWindow *owner;
		clan::GUIComponent *component;
		clan::GUITopLevelDescription description;
		clan::Rect box;
		bool enabled;
		bool visible;
	};

	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::GUIWindowManagerSite *site;
	std::map<clan::GUITopLevelWindow *, TopLevelWindow> windows;
	std::vector<clan::GUITopLevelWindow *> render_order;
	clan::GUITopLevelWindow *focus;
	clan::GUITopLevelWindow *mouse_capture;
	clan::SlotContainer slots;
	float scale;
	bool mouse_enabled;
	bool enabled;
#ifdef WIN32
	HCURSOR cursor;
#endif
	clan::Signal_v1<clan::InputEvent> _sig_mouse_input;
};
