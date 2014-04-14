#include "Consumable.h"
#include "../Engine.h"
#include "Actor.h"
#include "Container.h"
#include "Gui.h"

#include <iostream>

Consumable::Consumable(const EntityWPtr &owner) 
: Totem::Component<Consumable, PropertyUserData>("Consumable"), owner(owner)
{
	user_data.entity = owner;
	user_data.component = this;

	owner.lock()->registerToEvent1<EntityPtr>("Use").connect(this, &Consumable::use);
}

Consumable::~Consumable()
{
	//std::cout << "Consumable is being destroyed!" << std::endl;
}

void Consumable::use(EntityPtr wearer) {
	if(wearer->hasEvent("Remove",1))
		wearer->sendEvent1<EntityPtr>("Remove", owner.lock());

	Engine::getSingleton()->remove(owner.lock());

	if(wearer->hasComponent<Player>())
		Engine::getSingleton()->getGui()->message(TCODColor::lightViolet, "You quaff a %s.", owner.lock()->getName().c_str());
	else
		Engine::getSingleton()->getGui()->message(TCODColor::lightViolet, "%s quaffs a %s.", wearer->getName().c_str(), owner.lock()->getName().c_str());
}
