
#include "precomp.h"
#include "scaled_box.h"

using namespace clan;

ScaledBox::ScaledBox()
: anchor_left(anchor_min), anchor_top(anchor_min), anchor_right(anchor_min), anchor_bottom(anchor_min)
{
}

ScaledBox ScaledBox::top_left_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.top_left.x = x;
	box.top_left.y = y;
	box.bottom_right.x = x + width;
	box.bottom_right.y = y + height;
	return box;
}

ScaledBox ScaledBox::top_center_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_left = anchor_center;
	box.anchor_right = anchor_center;
	box.top_left.x = x - width / 2;
	box.top_left.y = y;
	box.bottom_right.x = x + width / 2;
	box.bottom_right.y = y + height;
	return box;
}

ScaledBox ScaledBox::top_right_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_left = anchor_max;
	box.anchor_right = anchor_max;
	box.top_left.x = -(x + width);
	box.top_left.y = y;
	box.bottom_right.x = -x;
	box.bottom_right.y = y + height;
	return box;
}

ScaledBox ScaledBox::center_left_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_left = anchor_min;
	box.anchor_right = anchor_min;
	box.anchor_top = anchor_center;
	box.anchor_bottom = anchor_center;
	box.top_left.x = x;
	box.top_left.y = y - height / 2;
	box.bottom_right.x = x + width;
	box.bottom_right.y = y + height / 2;
	return box;
}

ScaledBox ScaledBox::center_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_left = anchor_center;
	box.anchor_right = anchor_center;
	box.anchor_top = anchor_center;
	box.anchor_bottom = anchor_center;
	box.top_left.x = x - width / 2;
	box.top_left.y = y - height / 2;
	box.bottom_right.x = x + width / 2;
	box.bottom_right.y = y + height / 2;
	return box;
}

ScaledBox ScaledBox::center_right_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_left = anchor_max;
	box.anchor_right = anchor_max;
	box.anchor_top = anchor_center;
	box.anchor_bottom = anchor_center;
	box.top_left.x = -(x + width);
	box.top_left.y = y - height / 2;
	box.bottom_right.x = -x;
	box.bottom_right.y = y + height / 2;
	return box;
}

ScaledBox ScaledBox::bottom_left_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_top = anchor_max;
	box.anchor_bottom = anchor_max;
	box.top_left.x = x;
	box.top_left.y = -(y + height);
	box.bottom_right.x = x + width;
	box.bottom_right.y = -y;
	return box;
}

ScaledBox ScaledBox::bottom_center_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_left = anchor_center;
	box.anchor_right = anchor_center;
	box.anchor_top = anchor_max;
	box.anchor_bottom = anchor_max;
	box.top_left.x = x - width / 2;
	box.top_left.y = -(y + height);
	box.bottom_right.x = x + width / 2;
	box.bottom_right.y = -y;
	return box;
}

ScaledBox ScaledBox::bottom_right_box(float x, float y, float width, float height)
{
	ScaledBox box;
	box.anchor_left = anchor_max;
	box.anchor_right = anchor_max;
	box.anchor_top = anchor_max;
	box.anchor_bottom = anchor_max;
	box.top_left.x = -(x + width);
	box.top_left.y = -(y + height);
	box.bottom_right.x = -x;
	box.bottom_right.y = -y;
	return box;
}

Rect ScaledBox::get_geometry(Size parent, float scale)
{
	Point top_left_int((int)(top_left.x * scale + 0.5f), (int)(top_left.y * scale + 0.5f));
	Point bottom_right_int((int)(bottom_right.x * scale + 0.5f), (int)(bottom_right.y * scale + 0.5f));

	Rect box(
		get_anchor_pos(anchor_left, top_left_int.x, parent.width),
		get_anchor_pos(anchor_top, top_left_int.y, parent.height),
		get_anchor_pos(anchor_right, bottom_right_int.x, parent.width),
		get_anchor_pos(anchor_bottom, bottom_right_int.y, parent.height));

	return box;
}

int ScaledBox::get_anchor_pos(Anchor anchor, int offset, int size)
{
	switch (anchor)
	{
	default:
	case anchor_min:
		return offset;
	case anchor_max:
		return size + offset;
	case anchor_center:
		return size / 2 + offset;
	}
}
