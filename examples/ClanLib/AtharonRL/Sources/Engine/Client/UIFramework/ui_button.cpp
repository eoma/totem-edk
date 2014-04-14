
#include "precomp.h"
#include "ui_button.h"

using namespace clan;

UIButton::UIButton(GUIComponent *parent)
: UIComponent(parent), label(this)
{
	func_render().set(this, &UIButton::on_render);
	func_input_pressed().set(this, &UIButton::on_input_pressed);
	func_input_released().set(this, &UIButton::on_input_released);
	func_input_pointer_moved().set(this, &UIButton::on_input_pointer_moved);
	label.set_align(UILabel::text_align_center);
}

UIButton::UIButton(UIScreen *screen)
: UIComponent(screen), label(this)
{
	func_render().set(this, &UIButton::on_render);
	func_input_pressed().set(this, &UIButton::on_input_pressed);
	func_input_released().set(this, &UIButton::on_input_released);
	func_input_pointer_moved().set(this, &UIButton::on_input_pointer_moved);
	label.set_align(UILabel::text_align_center);
}

void UIButton::set_scaled_geometry(ScaledBox new_scaled_box)
{
	UIComponent::set_scaled_geometry(new_scaled_box);
	label.set_scaled_geometry(ScaledBox::top_left_box(0.0f, 0.0f, new_scaled_box.bottom_right.x - new_scaled_box.top_left.x, new_scaled_box.bottom_right.y - new_scaled_box.top_left.y));
}

void UIButton::on_render(Canvas &canvas, const Rect &update_box)
{
	Rect client_box = get_size();
	background.render(canvas, client_box, get_scale());
}

void UIButton::update_geometry()
{
	UIComponent::update_geometry();
	label.update_geometry();
}

bool UIButton::on_input_pressed(const InputEvent &input_event)
{
	if (input_event.id == mouse_left)
	{
		capture_mouse(true);
		return true;
	}
	else
	{
		return false;
	}
}

bool UIButton::on_input_released(const InputEvent &input_event)
{
	if (input_event.id == mouse_left)
	{
		capture_mouse(false);
		Rect box = get_size();
		if (box.contains(input_event.mouse_pos) && !cb_clicked.is_null())
			cb_clicked.invoke();
		return true;
	}
	else
	{
		return false;
	}
}

bool UIButton::on_input_pointer_moved(const InputEvent &input_event)
{
	return true;
}
