#pragma once

class Vec2i
{
public:
	Vec2i();
	Vec2i(const Vec2i &rhs);
	Vec2i(Vec2i &&rvalue);
	Vec2i(int x, int y);
	~Vec2i();

	int x() const { return x_; }
	int &x() { return x_; }

	int y() const { return y_; }
	int &y() { return y_; }

	int distance(const Vec2i &rhs) const;
	int distance(int x, int y) const;
	float distancef(const Vec2i &rhs) const;
	float distancef(int x, int y) const;
	float length() const;

	int round(double dbl) const;

	const Vec2i &normalize();
	Vec2i normalized() const;

	Vec2i clamp(int a, int b);
	static Vec2i clamp(const Vec2i &vec, int a, int b);
	static int clampi(int x, int a, int b);

	static bool overlap(const Vec2i &lower_bound_a, const Vec2i &upper_bound_a, const Vec2i &lower_bound_b, const Vec2i &upper_bound_b, Vec2i &overlap_vec, bool &count_x_from_left, bool &count_y_from_top);

	bool operator==(const Vec2i &rhs) const { return (x_ == rhs.x() && y_ == rhs.y()); }
	bool operator!=(const Vec2i &rhs) const { return (x_ != rhs.x() || y_ != rhs.y()); }
	bool operator<(const Vec2i &rhs) const { return x_ < rhs.x() && y_ < rhs.y(); }
	bool operator>(const Vec2i &rhs) const { return x_ > rhs.x() && y_ > rhs.y(); }
	bool operator<=(const Vec2i &rhs) const { return x_ <= rhs.x() && y_ <= rhs.y(); }
	bool operator>=(const Vec2i &rhs) const { return x_ >= rhs.x() && y_ >= rhs.y(); }

	Vec2i operator=(const Vec2i &rhs) { x_ = rhs.x(); y_ = rhs.y(); return *this; }
	Vec2i operator=(int rhs) { x_ = rhs; y_ = rhs; return *this; }

	Vec2i operator+(const Vec2i &rhs) const { return Vec2i(x_+rhs.x(), y_+rhs.y()); }
	Vec2i operator+(int rhs) const { return Vec2i(x_+rhs, y_+rhs); }

	Vec2i operator-(const Vec2i &rhs) const { return Vec2i(x_-rhs.x(), y_-rhs.y()); }
	Vec2i operator-(int rhs) const { return Vec2i(x_-rhs, y_-rhs); }

	Vec2i operator*(const Vec2i &rhs) const { return Vec2i(x_*rhs.x(), y_*rhs.y()); }
	Vec2i operator*(int rhs) const { return Vec2i(x_*rhs, y_*rhs); }

	Vec2i operator/(const Vec2i &rhs) const { return Vec2i(x_/rhs.x(), y_/rhs.y()); }
	Vec2i operator/(int rhs) const { return Vec2i(x_/rhs, y_/rhs); }

	Vec2i operator+=(const Vec2i &rhs) { x_ += rhs.x(); y_ += rhs.y(); return *this; }
	Vec2i operator+=(int rhs) { x_ += rhs, y_ += rhs; return *this; }

	Vec2i operator-=(const Vec2i &rhs) { x_ -= rhs.x(); y_ -= rhs.y(); return *this; }
	Vec2i operator-=(int rhs) { x_ -= rhs, y_ -= rhs; return *this; }

	Vec2i operator/=(const Vec2i &rhs) { x_ /= rhs.x(); y_ /= rhs.y(); return *this; }
	Vec2i operator/=(int rhs) { x_ /= rhs, y_ /= rhs; return *this; }

	Vec2i operator*=(const Vec2i &rhs) { x_ *= rhs.x(); y_ *= rhs.y(); return *this; }
	Vec2i operator*=(int rhs) { x_ *= rhs, y_ *= rhs; return *this; }
	
private:
	int x_,y_;
};
