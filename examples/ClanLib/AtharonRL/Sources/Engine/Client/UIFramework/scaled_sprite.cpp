
#include "precomp.h"
#include "scaled_sprite.h"

using namespace clan;

ScaledSprite::ScaledSprite()
: color(Colorf::transparent), slice_left(0), slice_top(0), slice_right(0), slice_bottom(0), fill_center(true),
  outset_left(0.0f), outset_top(0.0f), outset_right(0.0f), outset_bottom(0.0f),
  repeat_x(repeat_type_stretch), repeat_y(repeat_type_stretch), repeat_center(repeat_type_stretch),
  border_width_left(0.0f), border_width_top(0.0f), border_width_right(0.0f), border_width_bottom(0.0f)
{
}

void ScaledSprite::render(Canvas &canvas, Rect border_box, float scale)
{
	Rect border_image_area = get_border_image_area(border_box, scale);

	int grid_left = used_to_actual(border_width_left * scale);
	int grid_right = used_to_actual(border_width_right * scale);
	int grid_top = used_to_actual(border_width_top * scale);
	int grid_bottom = used_to_actual(border_width_bottom * scale);

	int x[4] = { border_image_area.left, border_image_area.left + grid_left, border_image_area.right - grid_right, border_image_area.right };
	int y[4] = { border_image_area.top, border_image_area.top + grid_top, border_image_area.bottom - grid_bottom, border_image_area.bottom };

	if (color.a != 0.0f)
		canvas.fill_rect(Rect(x[0], y[0], x[3], y[3]), color);

	if (image.is_null())
		return;

	int sx[4] = { 0, slice_left, image.get_width()-slice_right, image.get_width() };
	int sy[4] = { 0, slice_top, image.get_height()-slice_bottom, image.get_height() };

	for (int yy = 0; yy < 3; yy++)
	{
		for (int xx = 0; xx < 3; xx++)
		{
			bool center = (xx == 1 && yy == 1);
			if (!center || fill_center)
				draw_area(canvas, x[xx], y[yy], x[xx+1] - x[xx], y[yy+1] - y[yy], sx[xx], sy[yy], sx[xx+1] - sx[xx], sy[yy+1] - sy[yy], center);
		}
	}
}

void ScaledSprite::draw_area(Canvas &canvas, int x, int y, int w, int h, int sx, int sy, int sw, int sh, bool center)
{
	// To do: Support other repeat types than stretch

	if (w > 0 && h > 0)
		image.draw(canvas, Rect(sx, sy, sx + sw, sy + sh), Rect(x, y, x + w, y + h));
}

Rect ScaledSprite::get_border_image_area(const Rect &border_box, float scale) const
{
	Rect box = border_box;
	box.left -= used_to_actual(outset_left * scale);
	box.top -= used_to_actual(outset_top * scale);
	box.right += used_to_actual(outset_right * scale);
	box.bottom += used_to_actual(outset_bottom * scale);
	return box;
}
