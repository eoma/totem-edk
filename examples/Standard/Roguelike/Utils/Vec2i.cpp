#include "Vec2i.h"
#include <math.h>
Vec2i::Vec2i()
	: x_(0), y_(0)
{
}

Vec2i::Vec2i(const Vec2i &rhs)
	: x_(rhs.x_), y_(rhs.y_)
{
}

Vec2i::Vec2i(Vec2i &&rvalue)
	: x_(rvalue.x_), y_(rvalue.y_)
{
}

Vec2i::Vec2i(int x, int y)
	: x_(x), y_(y)
{
}

Vec2i::~Vec2i()
{
}

int Vec2i::distance(const Vec2i &rhs) const
{
	return (int)sqrtf(powf(((float)x_-(float)rhs.x()), 2.0f) + pow(((float)y_-(float)rhs.y()), 2.0f));
}

int Vec2i::distance(int x, int y) const
{
	return (int)sqrtf(powf(((float)x_-(float)x), 2.0f) + pow(((float)y_-(float)y), 2.0f));
}

float Vec2i::distancef(const Vec2i &rhs) const
{
	return sqrtf(powf(((float)x_-(float)rhs.x()), 2.0f) + pow(((float)y_-(float)rhs.y()), 2.0f));
}

float Vec2i::distancef(int x, int y) const
{
	return sqrtf(powf(((float)x_-(float)x), 2.0f) + pow(((float)y_-(float)y), 2.0f));
}

float Vec2i::length() const
{
	return sqrtf(powf((float)x_, 2.0f) + powf((float)y_, 2.0f));
}

int Vec2i::round(double dbl) const
{
	 return dbl >= 0.0 ? (int)(dbl + 0.5) : ((dbl - (double)(int)dbl) <= -0.5 ? (int)dbl : (int)(dbl - 0.5));
}

const Vec2i &Vec2i::normalize()
{
	auto dist = (double)length();
	x_ = (int)round((double)x_/dist);
	y_ = (int)round((double)y_/dist);
	return *this;
}

Vec2i Vec2i::normalized() const
{
	int x = 0, y = 0;
	auto dist = (double)length();
	x = (int)round((double)x_/dist);
	y = (int)round((double)y_/dist);
	return Vec2i(x,y);
}

Vec2i Vec2i::clamp(int a, int b)
{
	x_ = clampi(x_,a,b);
	y_ = clampi(y_,a,b);
	return *this;
}

Vec2i Vec2i::clamp(const Vec2i &vec, int a, int b)
{
	Vec2i C;
	C.x() = clampi(vec.x(), a,b);
	C.y() = clampi(vec.y(), a,b);
	return C;
}

int Vec2i::clampi(int x, int a, int b)
{
	return ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)));
}

bool Vec2i::overlap(const Vec2i &lower_bound_a, const Vec2i &upper_bound_a, const Vec2i &lower_bound_b, const Vec2i &upper_bound_b, Vec2i &overlap_vec, bool &count_x_from_left, bool &count_y_from_top)
{
	//Check if there's no overlap on X axis
	if(upper_bound_a.x() <= lower_bound_b.x() || upper_bound_b.x() <= lower_bound_a.x())
		return false;
	//Calculate overlap on X axis
	float a_center_x = (lower_bound_a.x() + upper_bound_a.x()) / 2.0f;
	float b_center_x = (lower_bound_b.x() + upper_bound_b.x()) / 2.0f;
	if(a_center_x < b_center_x) { overlap_vec.x() = upper_bound_a.x() - lower_bound_b.x(); count_x_from_left = false; }
	else						{ overlap_vec.x() = upper_bound_b.x() - lower_bound_a.x(); count_x_from_left = true;}

	//Check if there's no overlap on Y axis
	if(upper_bound_a.y() <= lower_bound_b.y() || upper_bound_b.y() <= lower_bound_a.y())
		return false; //Need to overlap on both X and Y axis for there to be an actual 2d overlap
	//Calculate overlap on Y axis
	float a_center_y = (lower_bound_a.y() + upper_bound_a.y()) / 2.0f;
	float b_center_y = (lower_bound_b.y() + upper_bound_b.y()) / 2.0f;
	if(a_center_y < b_center_y) { overlap_vec.y() = upper_bound_a.y() - lower_bound_b.y(); count_y_from_top = false; }
	else						{ overlap_vec.y() = upper_bound_b.y() - lower_bound_a.y(); count_y_from_top = true;}

	return true;
}
