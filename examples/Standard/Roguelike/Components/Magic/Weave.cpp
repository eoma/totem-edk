#include "Weave.h"
#include "../../Engine.h"
#include "../Actor.h"
#include "../Container.h"
#include "../Gui.h"

#include <iostream>

Weave::Weave(const EntityWPtr &owner) 
: Totem::Component<Weave, PropertyUserData>("Weave"), owner(owner)
{
	user_data.entity = owner;
	user_data.component = this;

	owner.lock()->registerToEvent1<EntityPtr>("Use").connect(this, &Weave::use);
}

Weave::~Weave()
{
	//std::cout << "Consumable is being destroyed!" << std::endl;
}

void Weave::use(EntityPtr wearer) {
	if(wearer->hasEvent("Remove",1))
		wearer->sendEvent1<EntityPtr>("Remove", owner.lock());

	Engine::getSingleton()->remove(owner.lock());

	if(wearer->hasComponent<Player>())
		Engine::getSingleton()->getGui()->message(TCODColor::lightCyan, "You weave your hands in intricate patterns.\nA %s crumbles to dust in your hand.", owner.lock()->getName().c_str());
	else
		Engine::getSingleton()->getGui()->message(TCODColor::lightCyan, "%s weave his hands in intricate patterns.\nA %s crumbles to dust in his hand.", wearer->getName().c_str(), owner.lock()->getName().c_str());
}
