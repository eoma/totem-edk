
#pragma once

class ScaledBox
{
public:
	enum Anchor
	{
		anchor_min,
		anchor_max,
		anchor_center
	};

	ScaledBox();

	static ScaledBox top_left_box(float x, float y, float width, float height);
	static ScaledBox top_center_box(float x, float y, float width, float height);
	static ScaledBox top_right_box(float x, float y, float width, float height);

	static ScaledBox center_left_box(float x, float y, float width, float height);
	static ScaledBox center_box(float x, float y, float width, float height);
	static ScaledBox center_right_box(float x, float y, float width, float height);

	static ScaledBox bottom_left_box(float x, float y, float width, float height);
	static ScaledBox bottom_center_box(float x, float y, float width, float height);
	static ScaledBox bottom_right_box(float x, float y, float width, float height);

	clan::Rect get_geometry(clan::Size parent, float scale);

	Anchor anchor_left, anchor_top, anchor_right, anchor_bottom;
	clan::Pointf top_left, bottom_right;

private:
	int get_anchor_pos(Anchor anchor, int offset, int size);
};
