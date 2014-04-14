
#pragma once

#include "ui_component.h"

class UILabel : public UIComponent
{
public:
	UILabel(clan::GUIComponent *parent);
	UILabel(UIScreen *screen);

	enum TextAlignment
	{
		text_align_left,
		text_align_center,
		text_align_right
	};

	void update_geometry();

	void set_font(std::string font_name, float size);
	void set_font_size(float size);
	void set_text(const std::string &new_text) { text = new_text; }
	void set_color(clan::Colorf new_color) { color = new_color; }
	void set_drop_shadow(bool enable) { drop_shadow = enable; }
	void set_align(TextAlignment align) { text_align = align; }

private:
	void update_font(clan::Canvas &canvas);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_box);

	std::string font_name;
	float size;
	std::string text;
	clan::Font font;
	clan::Colorf color;
	bool drop_shadow;
	TextAlignment text_align;
};
