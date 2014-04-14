#pragma once
#include <Totem/Totem.h>

class EntityManager;
class Entity;
struct Vec2i;

class Explosive : public Totem::Component<Explosive>
{
public:
	Explosive(Entity &owner, EntityManager &entity_manager);
	virtual ~Explosive() {}

	std::string get_type() const { return get_static_type(); }
	static std::string get_static_type() { return "Explosive"; }

private:
	Entity &owner;
	EntityManager &entity_manager;
	Totem::Property<int> explosion_radius_property;
	Totem::Property<int> explosion_damage_property;
	Totem::Property<Vec2i> position_property;

	void on_explode();
};