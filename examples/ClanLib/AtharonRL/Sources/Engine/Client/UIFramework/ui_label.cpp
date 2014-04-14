
#include "precomp.h"
#include "ui_label.h"

using namespace clan;

UILabel::UILabel(GUIComponent *parent)
: UIComponent(parent), font_name("Segoe Print"), size(13.0f), color(Colorf::white), drop_shadow(true), text_align(text_align_left)
{
	func_render().set(this, &UILabel::on_render);
}

UILabel::UILabel(UIScreen *screen)
: UIComponent(screen), font_name("Segoe Print"), size(13.0f), color(Colorf::white), drop_shadow(true), text_align(text_align_left)
{
	func_render().set(this, &UILabel::on_render);
}

void UILabel::on_render(Canvas &canvas, const Rect &update_box)
{
	if (!text.empty())
	{
		if (font.is_null())
			update_font(canvas);

		int x = 0;
		int y = (int)font.get_font_metrics().get_ascent();

		if (text_align == text_align_center)
		{
			x = max((get_width() - font.get_text_size(canvas.get_gc(), text).width) / 2, 0);
		}
		else if (text_align == text_align_right)
		{
			x = max(get_width() - font.get_text_size(canvas.get_gc(), text).width - 1, 0);
		}

		if (drop_shadow)
			font.draw_text_ellipsis(canvas, x + 1, y + 1, get_size(), text, Colorf::black);
		font.draw_text_ellipsis(canvas, x, y, get_size(), text, color);
	}
}

void UILabel::update_geometry()
{
	UIComponent::update_geometry();
	Canvas canvas = get_canvas();
	update_font(canvas);
}

void UILabel::update_font(Canvas &canvas)
{
	if (!text.empty())
		font = Font(canvas, font_name, (int)(-size * get_scale() - 0.5f));
	else
		font = Font();
}

void UILabel::set_font(std::string new_font_name, float new_size)
{
	font_name = new_font_name;
	size = new_size;
	font = Font();
}

void UILabel::set_font_size(float new_size)
{
	size = new_size;
	font = Font();
}
