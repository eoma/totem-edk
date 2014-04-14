/*Totem EDK
Copyright (c) 2009 Pål Trefall and Kenneth Gangstø

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Note: Some of the libraries Totem EDK may link to may have additional
requirements or restrictions.
*/

/*#include <Totem/Component.h>
#include <Totem/ComponentContainer.h>
#include <Totem/Property.h>
#include <Totem/PropertyContainer.h>
#include <Totem/PropertyList.h>
#include <Totem/PropertyListContainer.h>
#include <Totem/HashedString.h>*/
#include <Totem/Totem.h>

#include <memory>
#include <algorithm>
#include <iostream>

/*
	Disclaimer:
	In this test we'll run through all the functionality in Totem and catch implementation bugs, so expect this file to be
	a bit messy in terms of all the function calls and setup. Refer to the examples to see
	clean code.
*/

class Entity
	:	public Totem::ComponentContainer<>,
		public Totem::PropertyContainer<>,
		public Totem::PropertyListContainer<>,
		public sigslot::has_slots<>
{
public:
	Entity(Totem::HashedString name) : name(name) {
		componentAdded().connect(this, &Entity::onComponentAdded);
		componentRemoved().connect(this, &Entity::onComponentRemoved);
		std::cout << "Entity " << getName() << ", has been instantiated." << std::endl;
	}
	virtual ~Entity() {
		std::cout << "Entity " << getName() << " is being destroyed!" << std::endl;
	}
	const std::string &getName() const { return name.getStr(); }
	unsigned int getId() const { return name.getId(); }

	void onComponentAdded(std::shared_ptr<Totem::IComponent<>> component) {
		std::cout << "The component " << component->getName() << " was added to the entity " << getName() << "." << std::endl;
	}
	void onComponentRemoved(std::shared_ptr<Totem::IComponent<>> component) {
		std::cout << "The component " << component->getName() << " was removed from the entity " << getName() << "." << std::endl;
	}

private:
	Totem::HashedString name;
};
typedef std::shared_ptr<Entity> EntityPtr;

class EntityWithUserData;
struct PropertyUserData
{
	EntityWithUserData *entity;
	Totem::IComponent<PropertyUserData> *component;
};

class EntityWithUserData 
	:	public Totem::ComponentContainer<PropertyUserData>, 
		public Totem::PropertyContainer<PropertyUserData>, 
		public Totem::PropertyListContainer<PropertyUserData>,
		public sigslot::has_slots<>
{
public:	
	EntityWithUserData(const std::string &name) : name(name) {
		componentAdded().connect(this, &EntityWithUserData::onComponentAdded);
		componentRemoved().connect(this, &EntityWithUserData::onComponentRemoved);
		std::cout << "EntityWithUserData " << getName() << ", has been instantiated." << std::endl;
	}
	virtual ~EntityWithUserData() {
		std::cout << "EntityWithUserData " << name << " is being destroyed!" << std::endl;
	}
	const std::string &getName() const { return name; }

	void onComponentAdded(std::shared_ptr<Totem::IComponent<PropertyUserData>> component) {
		std::cout << "The component " << component->getName() << " was added to the entity " << getName() << "." << std::endl;
	}
	void onComponentRemoved(std::shared_ptr<Totem::IComponent<PropertyUserData>> component) {
		std::cout << "The component " << component->getName() << " was removed from the entity " << getName() << "." << std::endl;
	}
private:
	std::string name;
};
typedef std::shared_ptr<EntityWithUserData> EntityWithUserDataPtr;

class MyComponent : public Totem::Component<MyComponent>
{
public:
	MyComponent(Entity *owner, Totem::HashedString name)
		: Component<MyComponent>(name.getStr()), id(name.getId()), owner(owner)
	{
		my_shared_string_property = owner->add<std::string>("MySharedString", "Testing shared string");
		my_private_int_property = add<int>("MyPrivateInt", 3);
		my_shared_int_property_list = owner->addList<int>("MySharedIntList");
		Totem::IProperty::isType<std::string>(my_shared_string_property);
		
		my_shared_string_property.valueChanged().connect(this, &MyComponent::onMySharedStringPropertyChanged);
		my_private_int_property.valueChanged().connect(this, &MyComponent::onMyPrivateIntPropertyChanged);
		my_shared_int_property_list.valueChanged().connect(this, &MyComponent::onMySharedIntPropertyListValueChanged);
		my_shared_int_property_list.valueAdded().connect(this, &MyComponent::onMySharedInPropertyListValueAdded);
		my_shared_int_property_list.valueErased().connect(this, &MyComponent::onMySharedInPropertyListValueErased);
		my_shared_int_property_list.valuesCleared().connect(this, &MyComponent::onMySharedInPropertyListCleared);
		my_shared_int_property_list.listResized().connect(this, &MyComponent::onMySharedInPropertyListResized);

		std::cout << "Component " << getName() << ", owned by " << owner->getName() << ", has been instantiated." << std::endl;
	}
	virtual ~MyComponent() {}

	unsigned int getId() const { return id; }

private:
	unsigned int id;
	Entity *owner;

	void onMySharedStringPropertyChanged(const std::string &old_value, const std::string &new_value) { 
		std::cout << "My shared string property changed from " << old_value << " to " << new_value << "." << std::endl;
	}
	void onMyPrivateIntPropertyChanged(const int &old_value, const int &new_value) {
		std::cout << "My private int property changed from " << old_value << " to " << new_value << "." << std::endl;
	}
	void onMySharedIntPropertyListValueChanged(unsigned int index, const int &old_value, const int &new_value) {
		std::cout << "My shared int property list changed value at index " << index << " from " << old_value << " to " << new_value << "." << std::endl;
	}
	void onMySharedInPropertyListValueAdded(unsigned int index, const int &value) {
		std::cout << "My shared int property list added a new value at index " << index << " with value " << value << "." << std::endl;
	}
	void onMySharedInPropertyListValueErased(unsigned int index, const int &value) {
		std::cout << "My shared int property list erased it's value at index " << index << " with value " << value << "." << std::endl;
	}
	void onMySharedInPropertyListCleared() { 
		std::cout << "My shared int property list was cleared." << std::endl; 
	}
	void onMySharedInPropertyListResized(unsigned int old_size, unsigned int new_size) {
		std::cout << "My shared int property list was resized from " << old_size << " to " << new_size << "." << std::endl;
	}

	Totem::Property<std::string> my_shared_string_property;
	Totem::Property<int> my_private_int_property;
	Totem::PropertyList<int> my_shared_int_property_list;
};
typedef std::shared_ptr<MyComponent> MyComponentPtr;

class MySecondComponent : public Totem::Component<MySecondComponent>
{
public:
	MySecondComponent(Entity *owner, const std::string &name) : Totem::Component<MySecondComponent>(name), owner(owner) {
		std::cout << "Component " << getName() << ", owned by " << owner->getName() << ", has been instantiated." << std::endl;
	}
	virtual ~MySecondComponent() {
	}

private:
	Entity *owner;
};
typedef std::shared_ptr<MySecondComponent> MySecondComponentPtr;

int main()
{
	EntityPtr my_first_entity = std::make_shared<Entity>("MyFirstEntity");
	{
		auto my_first_component = my_first_entity->addComponent<MyComponent>(std::make_shared<MyComponent>(my_first_entity.get(), "MyFirstComponent"));
		std::cout << "Id of component " << my_first_component->getName() << " of entity " << my_first_entity->getName() << " is " << my_first_component->getId() << "." << std::endl;
		auto my_second_component_interface = my_first_entity->addComponent(std::make_shared<MySecondComponent>(my_first_entity.get(), "MySecondComponent"));
		Totem::IComponent<void*>::isType<MyComponent>(my_first_component);
		Totem::Component<MyComponent, void*>::isType<MyComponent>(my_first_component);
	}
	EntityPtr my_second_entity = std::make_shared<Entity>("MySecondEntity");
	system("pause");
	return 0;
}