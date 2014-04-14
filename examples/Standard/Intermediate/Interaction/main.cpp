
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
// This example illustrate interaction between
// entities via components.
//
//
#include "Entity.h"
#include "EntityManager.h"
#include "Transform.h"
#include "Health.h"
#include "Explosive.h"
#include "Definitions.h"
#include "Vec2i.h"

EntityPtr make_box(const std::string &name, EntityManager &mgr)
{
	auto box = mgr.add_entity(name);
	box->addComponent(std::make_shared<Transform>(*box.get()));
	box->addComponent(std::make_shared<Health>(*box.get(), mgr));
	return box;
}

EntityPtr make_bomb(const std::string &name, EntityManager &mgr)
{
	auto bomb = mgr.add_entity(name);
	bomb->addComponent(std::make_shared<Transform>(*bomb.get()));
	bomb->addComponent(std::make_shared<Health>(*bomb.get(), mgr));
	bomb->addComponent(std::make_shared<Explosive>(*bomb.get(), mgr));
	return bomb;
}

int main(int /*argc*/, char** /*argv*/)
{
	std::cout << "--------------------------" << std::endl;
	std::cout << "EXPLODING BOMBS EXAMPLE!" << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "This example displays the interaction between multiple entities via uncoupled components." << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "CREATING ENTITIES" << std::endl;
	std::cout << "--------------------------" << std::endl;
	EntityManager mgr;
	auto boxA = make_box("BoxA", mgr);
	auto boxB = make_box("BoxB", mgr);
	auto bombA = make_bomb("BombA", mgr);
	auto bombB = make_bomb("BombB", mgr);

	std::cout << "--------------------------" << std::endl;
	std::cout << "SETTING PROPERTIES" << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "Position of " << bombA->get_name() << " is " << bombA->get<Vec2i>(PROPERTY_POSITION).get().x << ", " << bombA->get<Vec2i>(PROPERTY_POSITION).get().y << std::endl;
	bombB->get<Vec2i>(PROPERTY_POSITION) = Vec2i(2,0);
	boxA->get<Vec2i>(PROPERTY_POSITION) = Vec2i(2,1);
	boxB->get<Vec2i>(PROPERTY_POSITION) = Vec2i(5,0);

	boxA->get<int>(PROPERTY_HEALTH) = 150;
	boxB->get<int>(PROPERTY_HEALTH) = 150;
	std::cout << "Health of " << bombA->get_name() << " is " << bombA->get<int>(PROPERTY_HEALTH) << std::endl;
	std::cout << "Health of " << bombB->get_name() << " is " << bombB->get<int>(PROPERTY_HEALTH) << std::endl;

	std::cout << "--------------------------" << std::endl;
	std::cout << "START EXPLOSION" << std::endl;
	std::cout << "--------------------------" << std::endl;
	bombA->get<int>(PROPERTY_HEALTH) = 0;

	std::cout << "--------------------------" << std::endl;
	std::cout << "UPDATE ENTITY MANAGER STATE" << std::endl;
	std::cout << "--------------------------" << std::endl;
	mgr.update();

	std::cout << "--------------------------" << std::endl;
	std::cout << "ENTITIES IN ENTITY MANAGER" << std::endl;
	std::cout << "--------------------------" << std::endl;
	for(auto entity : mgr.get_entities())
	{
		std::cout << entity->get_name() << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	system("pause");
	return 0;
}
