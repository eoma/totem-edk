
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
// Property and PropertyContainer in Totem.
//
//

//---------------------------------------------------------------
//													 E N T I T Y

class Entity : public Totem::PropertyContainer<>, public sigslot::has_slots<>
{
public:
	Entity(const std::string &name) 
		: name(name) 
	{
		propertyAdded().connect(this, &Entity::on_property_added);
		propertyRemoved().connect(this, &Entity::on_property_removed);

		std::cout << name << " was created" << std::endl;
	}
	virtual ~Entity() {}

	const std::string &get_name() const { return name; }

	void on_basic_property_changed(const int &old_value, const int &new_value) 
	{ 
		std::cout << "BasicProperty changed from " << old_value << " to " << new_value << std::endl; 
	}

private:
	void on_property_added(std::shared_ptr<Totem::IProperty> property) { std::cout << property->getName() << " was added to " << name << std::endl; }
	void on_property_removed(std::shared_ptr<Totem::IProperty> property) { std::cout << property->getName() << " was removed from " << name << std::endl; }

	std::string name;
};

//---------------------------------------------------------------
//														 M A I N

int main(int /*argc*/, char** /*argv*/)
{
	Entity entity("BasicEntity");
	
	entity.add<int>("BasicProperty", 0)
		.valueChanged().connect(&entity, &Entity::on_basic_property_changed);
	
	if(entity.hasProperty("BasicProperty"))
	{
		auto property = entity.get<int>("BasicProperty");
		std::cout << entity.get_name() << " has " << property.getName() << " with value " << property.get() << std::endl;
		property = 1337;
	}
	entity.removeProperty("BasicProperty");
	
	system("pause");
	return 0;
}
