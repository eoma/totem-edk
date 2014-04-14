#pragma once

#include <cmath>

struct Vec2i
{
	int x;
	int y;
	Vec2i() : x(0), y(0) {}
	Vec2i(int x, int y) : x(x), y(y) {}

	int length() const { return length(*this); }

	int distance(const Vec2i &v)
	{
		return distance(*this, v);
	}

	static int length(const Vec2i &v) { return sqrt(v.x*v.x + v.y*v.y); }
	static int distance(const Vec2i &a, const Vec2i &b) { return length(Vec2i(a.x - b.x, a.y - b.y)); }

	bool operator !=(const Vec2i &rhs)
	{
		return ( (x != rhs.x) || (y != rhs.y) );
	}
};
