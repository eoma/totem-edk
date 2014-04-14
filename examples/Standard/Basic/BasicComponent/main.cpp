
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
// Component and ComponentContainer in Totem.
//
//

//---------------------------------------------------------------
//													 E N T I T Y

class Entity : public Totem::ComponentContainer<>, public sigslot::has_slots<>
{
public:
	Entity(const std::string &name) 
		: name(name) 
	{
		componentAdded().connect(this, &Entity::on_component_added);
		componentRemoved().connect(this, &Entity::on_component_removed);

		std::cout << name << " was created" << std::endl;
	}
	virtual ~Entity() {}

	const std::string &get_name() const { return name; }

private:
	void on_component_added(std::shared_ptr<Totem::IComponent<>> component) { std::cout << component->getName() << " was added to " << name << std::endl; }
	void on_component_removed(std::shared_ptr<Totem::IComponent<>> component) { std::cout << component->getName() << " was removed from " << name << std::endl; }

	std::string name;
};

//---------------------------------------------------------------
//												C O M P O N E N T

class BasicComponent : public Totem::Component<BasicComponent>
{
public:
	BasicComponent() 
		: Totem::Component<BasicComponent>(get_type()) 
	{
		std::cout << getName() << " was created" << std::endl;
	}
	virtual ~BasicComponent() {}

	static std::string get_type() { return "BasicComponent"; }

	void update(float /*elapsedTime*/) override { std::cout << "Updated " + this->getName() << std::endl; }
};

//---------------------------------------------------------------
//														 M A I N

int main(int /*argc*/, char** /*argv*/)
{
	Entity entity("BasicEntity");
	entity.addComponent( std::make_shared<BasicComponent>() );
	entity.updateComponents(0);
	if(entity.hasComponent<BasicComponent>())
	{
		auto component = entity.getComponent<BasicComponent>();
		std::cout << entity.get_name() << " has component " << component->getName() << std::endl;
	}
	entity.removeComponent<BasicComponent>();
	
	system("pause");
	return 0;
}
