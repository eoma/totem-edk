
#include "precomp.h"
#include "window_manager.h"
#include "ui_component.h"
#include <algorithm>

using namespace clan;

WindowManagerProvider::WindowManagerProvider(DisplayWindow window)
: window(window), canvas(window.get_gc()), site(0), focus(0), mouse_capture(0), scale(1.0f), mouse_enabled(true), enabled(false)
{
	slots.connect(window.sig_resize(), this, &WindowManagerProvider::on_displaywindow_resize);

	InputContext ic = window.get_ic();
	slots.connect(ic.get_mouse().sig_key_up(), this, &WindowManagerProvider::on_mouse_input_received);
	slots.connect(ic.get_mouse().sig_key_down(), this, &WindowManagerProvider::on_mouse_input_received);
	slots.connect(ic.get_mouse().sig_key_dblclk(), this, &WindowManagerProvider::on_mouse_input_received);
	slots.connect(ic.get_mouse().sig_pointer_move(), this, &WindowManagerProvider::on_mouse_input_received);
	slots.connect(ic.get_keyboard().sig_key_up(), this, &WindowManagerProvider::on_input_received);
	slots.connect(ic.get_keyboard().sig_key_down(), this, &WindowManagerProvider::on_input_received);

	for (int i = 0; i < ic.get_tablet_count(); ++i)
	{
		slots.connect(ic.get_tablet(i).sig_axis_move(), this, &WindowManagerProvider::on_mouse_input_received);
		slots.connect(ic.get_tablet(i).sig_key_down(), this, &WindowManagerProvider::on_mouse_input_received);
		slots.connect(ic.get_tablet(i).sig_key_dblclk(), this, &WindowManagerProvider::on_mouse_input_received);
		slots.connect(ic.get_tablet(i).sig_key_up(), this, &WindowManagerProvider::on_mouse_input_received);
		slots.connect(ic.get_tablet(i).sig_proximity_change(), this, &WindowManagerProvider::on_mouse_input_received);
	}

#ifdef WIN32
	DataBuffer ani_file = File::read_bytes("Resources/Engine/Cursors/NormalSelect.ani");
	int desired_width = 32;
	int desired_height = 32;
	cursor = (HCURSOR)CreateIconFromResourceEx((PBYTE) ani_file.get_data(), ani_file.get_size(), FALSE, 0x00030000, desired_width, desired_height, LR_DEFAULTCOLOR);
	if (cursor == 0)
		throw Exception("CreateIconFromResourceEx failed!");
#endif
}

WindowManagerProvider::~WindowManagerProvider()
{
	window.set_cursor(cursor_arrow);
#ifdef WIN32
	DestroyIcon(cursor);
#endif
}

void WindowManagerProvider::enable(bool enable)
{
	if (enabled != enable && mouse_enabled)
	{
		if (enable)
		{
			window.show_cursor();
			window.set_cursor_handle(cursor);
		}
		else
		{
			window.hide_cursor();
		}
	}
	enabled = enable;
}

void WindowManagerProvider::enable_mouse(bool enable)
{
	if (mouse_enabled != enable && enabled)
	{
		if (enable)
		{
			window.show_cursor();
			SetCursor(cursor);
		}
		else
		{
			window.hide_cursor();
		}
	}
	mouse_enabled = enable;
}

void WindowManagerProvider::set_scale(float new_scale)
{
	if (new_scale != scale)
	{
		scale = new_scale;
		update_geometry();
	}
}

void WindowManagerProvider::update_geometry()
{
	for (std::map<GUITopLevelWindow *, TopLevelWindow>::iterator it = windows.begin(); it != windows.end(); ++it)
	{
		UIComponent *ui_component = dynamic_cast<UIComponent*>(it->second.component);
		if (ui_component)
			ui_component->update_geometry();
	}
}

float WindowManagerProvider::get_scale() const
{
	return scale;
}

GUIWindowManager::WindowManagerType WindowManagerProvider::get_window_manager_type() const
{
	return GUIWindowManager::cl_wm_type_texture;
}

void WindowManagerProvider::destroy()
{
	delete this;
}

void WindowManagerProvider::set_site(GUIWindowManagerSite *new_site)
{
	site = new_site;
}

void WindowManagerProvider::create_window(GUITopLevelWindow *handle, GUITopLevelWindow *owner, GUIComponent *component, GUITopLevelDescription description)
{
	windows[handle] = TopLevelWindow(owner, component, description);
	render_order.push_back(handle);
}

void WindowManagerProvider::destroy_window(GUITopLevelWindow *handle)
{
	if (focus == handle)
		focus = 0;
	if (mouse_capture == handle)
	{
		window.capture_mouse(false);
		mouse_capture = 0;
	}
	render_order.erase(std::find(render_order.begin(), render_order.end(), handle));
	windows.erase(windows.find(handle));
}

void WindowManagerProvider::enable_window(GUITopLevelWindow *handle, bool enable)
{
	windows[handle].enabled = enable;
}

bool WindowManagerProvider::has_focus(GUITopLevelWindow *handle) const
{
	return focus == handle;
}

void WindowManagerProvider::set_visible(GUITopLevelWindow *handle, bool visible, bool activate_root_win)
{
	windows[handle].visible = visible;
	if (activate_root_win)
		activate(handle);
}

void WindowManagerProvider::activate(GUITopLevelWindow *handle)
{
	if (focus != handle)
	{
		if (focus)
			site->func_focus_lost->invoke(focus);
		focus = handle;
		if (focus)
			site->func_focus_gained->invoke(focus);
	}
}

void WindowManagerProvider::deactivate()
{
	if (focus)
		site->func_focus_lost->invoke(focus);
	focus = 0;
}

void WindowManagerProvider::set_geometry(GUITopLevelWindow *handle, const Rect &geometry, bool client_area)
{
	windows[handle].box = geometry;
}

Rect WindowManagerProvider::get_geometry(GUITopLevelWindow *handle, bool client_area) const
{
	return windows.find(handle)->second.box;
}

Point WindowManagerProvider::screen_to_window(GUITopLevelWindow *handle, const Point &screen_point, bool client_area) const
{
	return screen_point - windows.find(handle)->second.box.get_top_left();
}

Point WindowManagerProvider::window_to_screen(GUITopLevelWindow *handle, const Point &window_point, bool client_area) const
{
	return windows.find(handle)->second.box.get_top_left() + window_point;
}

Canvas& WindowManagerProvider::get_canvas(GUITopLevelWindow *handle)
{
	return canvas;
}

InputContext WindowManagerProvider::get_ic(GUITopLevelWindow *handle) const
{
	return window.get_ic();
}

Canvas WindowManagerProvider::begin_paint(GUITopLevelWindow *handle, const Rect &update_region)
{
	canvas.set_modelview(Mat4f::translate((float)windows[handle].box.left, (float)windows[handle].box.top, 0.0f));
	return canvas;
}

void WindowManagerProvider::end_paint(Canvas &canvas, GUITopLevelWindow *handle, const Rect &update_region)
{
	canvas.set_modelview(Mat4f::identity());
	canvas.flush();
}

void WindowManagerProvider::set_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	Rect box = rect;
	box.translate(windows[handle].box.get_top_left());
	canvas.set_cliprect(box);
}

void WindowManagerProvider::reset_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.reset_cliprect();
}

void WindowManagerProvider::push_cliprect(GUITopLevelWindow *handle, Canvas &canvas, const Rect &rect)
{
	Rect box = rect;
	box.translate(windows[handle].box.get_top_left());
	canvas.push_cliprect(box);
}

void WindowManagerProvider::pop_cliprect(GUITopLevelWindow *handle, Canvas &canvas)
{
	canvas.pop_cliprect();
}

void WindowManagerProvider::request_repaint(GUITopLevelWindow *handle, const Rect &update_region)
{
}

void WindowManagerProvider::bring_to_front(GUITopLevelWindow *handle)
{
}

bool WindowManagerProvider::is_minimized(GUITopLevelWindow *handle) const
{
	return false;
}

bool WindowManagerProvider::is_maximized(GUITopLevelWindow *handle) const
{
	return false;
}

void WindowManagerProvider::capture_mouse(GUITopLevelWindow *handle, bool state)
{
	mouse_capture = state ? handle : (GUITopLevelWindow *)0;
	window.capture_mouse(state);
}

DisplayWindow WindowManagerProvider::get_display_window(GUITopLevelWindow *handle) const
{
	return window;
}

void WindowManagerProvider::set_cursor(GUITopLevelWindow *handle, const Cursor &cursor)
{
	// To do: only do this for when the mouse is over the specific window
	if (enabled)
		window.set_cursor(cursor);
}

void WindowManagerProvider::set_cursor(GUITopLevelWindow *handle, enum StandardCursor type)
{
	// To do: only do this for when the mouse is over the specific window
	if (enabled)
	{
		if (type == cursor_arrow)
			window.set_cursor_handle(cursor);
		else
			window.set_cursor(type);
	}
}

void WindowManagerProvider::update()
{
}

void WindowManagerProvider::setup_painting()
{
}

void WindowManagerProvider::complete_painting()
{
}

void WindowManagerProvider::on_input_received(const InputEvent &e)
{
	if (enabled)
	{
		if (mouse_capture)
			invoke_input_received(mouse_capture, e);
		else if (focus)
			invoke_input_received(focus, e);
	}
}

void WindowManagerProvider::on_mouse_input_received(const InputEvent &e)
{
	if (enabled)
	{
		GUITopLevelWindow *mouse_over = get_window_at_point(e.mouse_pos);
		if (mouse_capture)
			invoke_input_received(mouse_capture, e);
		else if (mouse_over)
			invoke_input_received(mouse_over, e);
		else
			sig_mouse_input().invoke(e);
	}
}

void WindowManagerProvider::on_displaywindow_resize(int, int)
{
	update_geometry();
}

GUITopLevelWindow *WindowManagerProvider::get_window_at_point(const Point &point)
{
	std::vector<GUITopLevelWindow *>::size_type index, size;
	size = render_order.size();
	for (index = 0; index < size; index++)
	{
		TopLevelWindow toplevel_window = windows[render_order[index]];
		if (toplevel_window.visible && toplevel_window.box.contains(point))
		{
			return render_order[index];
		}
	}
	return 0;
}

void WindowManagerProvider::invoke_input_received(GUITopLevelWindow *window, const InputEvent &input_event)
{
	InputEvent inp_event = input_event;
	inp_event.mouse_pos.x -= windows[window].box.left;
	inp_event.mouse_pos.y -= windows[window].box.top;
	site->func_input_received->invoke(window, inp_event);
}
