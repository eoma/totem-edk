#pragma once
#include <Totem/Totem.h>

class Entity;
class EntityManager;

class Health : public Totem::Component<Health>
{
public:
	Health(Entity &owner, EntityManager &entity_manager);
	virtual ~Health() {}

	std::string get_type() const { return get_static_type(); }
	static std::string get_static_type() { return "Health"; }

private:
	Entity &owner;
	EntityManager &entity_manager;
	Totem::Property<int> health_property;
	bool alive;

	void on_health_changed(const int &old_value, const int &new_value);
	void on_take_damage(int damage);
};