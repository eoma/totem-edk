
#pragma once

#include <Totem/EventSystem.h>

class GameObject : public Totem::ComponentContainer<>, public Totem::PropertyContainer<>, public Totem::Ext::EventSystem<>
{
public:
	GameObject(int id, const std::string &name);
	virtual ~GameObject() { }

	virtual void update(float time_elapsed);

	bool is_destroyed() const;
	void destroy();

	int get_id() const;
	std::string get_name() const;

protected:
	int id;

	Totem::Property<std::string> property_name;

private:
	bool destroyed;
};
