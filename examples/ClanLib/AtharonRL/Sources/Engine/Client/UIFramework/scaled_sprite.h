
#pragma once

class ScaledSprite
{
public:
	ScaledSprite();
	void render(clan::Canvas &canvas, clan::Rect box, float scale);

	void set_slice(int left, int top, int right, int bottom)
	{
		slice_left = left;
		slice_top = top;
		slice_right = right;
		slice_bottom = bottom;
	}

	void set_outset(float left, float top, float right, float bottom)
	{
		outset_left = left;
		outset_top = top;
		outset_right = right;
		outset_bottom = bottom;
	}

	void set_border_width(float left, float top, float right, float bottom)
	{
		border_width_left = left;
		border_width_top = top;
		border_width_right = right;
		border_width_bottom = bottom;
	}

	clan::Image image;
	clan::Colorf color;

	int slice_left;
	int slice_top;
	int slice_right;
	int slice_bottom;
	bool fill_center;

	float outset_left;
	float outset_top;
	float outset_right;
	float outset_bottom;

	enum RepeatType
	{
		repeat_type_stretch,
		repeat_type_repeat,
		repeat_type_round,
		repeat_type_space
	};
	RepeatType repeat_x;
	RepeatType repeat_y;
	RepeatType repeat_center;

	float border_width_left;
	float border_width_top;
	float border_width_right;
	float border_width_bottom;

private:
	void draw_area(clan::Canvas &canvas, int x, int y, int w, int h, int sx, int sy, int sw, int sh, bool center);
	clan::Rect get_border_image_area(const clan::Rect &border_box, float scale) const;

	typedef float CSSUsedValue;
	typedef int CSSActualValue;
	static inline CSSActualValue used_to_actual(CSSUsedValue v) { return (CSSActualValue)(v >= 0.0f ? v+0.1f : v-0.1f); }
};
