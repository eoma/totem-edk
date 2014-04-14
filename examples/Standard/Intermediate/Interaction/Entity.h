#pragma once
#include <Totem/Totem.h>
#include <Totem/Extensions/EventSystem.h>

class Entity : public Totem::PropertyContainer<>, public Totem::ComponentContainer<>, public Totem::EventSystem<>
{
public:
	Entity(const std::string &name);
	virtual ~Entity() {}

	const std::string &get_name() const { return name; }

private:
	void on_property_added(std::shared_ptr<Totem::IProperty> property);
	void on_property_removed(std::shared_ptr<Totem::IProperty> property);
	void on_component_added(std::shared_ptr<Totem::IComponent<>> component);
	void on_component_removed(std::shared_ptr<Totem::IComponent<>> component);

	std::string name;
};