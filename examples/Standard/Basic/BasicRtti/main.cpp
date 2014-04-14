
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

#include <Totem/Totem.h>
#include <iostream>

//---------------------------------------------------------------
//														A B O U T
//
// This example illustrate the basic usage of
// run-time type identification with Property and
// Component in Totem.
//
//

//---------------------------------------------------------------
//													 E N T I T Y

class Entity : public Totem::PropertyContainer<>, public Totem::ComponentContainer<>, public sigslot::has_slots<>
{
public:
	Entity(const std::string &name)	: name(name) {}
	virtual ~Entity() {}

	const std::string &get_name() const { return name; }

private:
	std::string name;
};

//---------------------------------------------------------------
//												C O M P O N E N T

class BasicComponent : public Totem::Component<BasicComponent>
{
public:
	BasicComponent(Entity &owner) 
		: Totem::Component<BasicComponent>(get_type()) 
	{
		basic_property = owner.add<int>("BasicProperty", 0);
	}
	virtual ~BasicComponent() {}

	static std::string get_type() { return "BasicComponent"; }

private:
	Totem::Property<int> basic_property;
};

//---------------------------------------------------------------
//														 M A I N

int main(int /*argc*/, char** /*argv*/)
{
	Entity entity("BasicEntity");
	auto component = entity.addComponent( std::make_shared<BasicComponent>(entity) );
	
	if(Totem::IComponent<>::isType<BasicComponent>(component))
	{
		std::cout << component->getName() << " is of component type " << BasicComponent::get_type() << std::endl;
	}
	
	for( auto name_property_pair : entity.getProperties() )
	{
		auto property_name = name_property_pair.first;
		auto property = name_property_pair.second;

		if(Totem::IProperty::isType<int>(property))
		{
			std::cout << property_name << " is of property type int" << std::endl;
		}
	}

	system("pause");
	return 0;
}
