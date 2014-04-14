#pragma once
#include <Totem/Totem.h>

class Entity;
struct Vec2i;

class Transform : public Totem::Component<Transform>
{
public:
	Transform(Entity &owner);
	virtual ~Transform() {}

	std::string get_type() const { return get_static_type(); }
	static std::string get_static_type() { return "Transform"; }

private:
	Entity &owner;
	Totem::Property<Vec2i> position_property;

	void on_position_changed(const Vec2i &old_value, const Vec2i &new_value);
};