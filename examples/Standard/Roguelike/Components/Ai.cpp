#include "Ai.h"
#include "../Engine.h"

#include <iostream>

Ai::Ai(const EntityWPtr &owner) 
: Totem::Component<Ai, PropertyUserData>("Ai"), owner(owner)
{
	user_data.entity = owner;
	user_data.component = this;

}

Ai::~Ai()
{
	//std::cout << "Ai is being destroyed!" << std::endl;
}
