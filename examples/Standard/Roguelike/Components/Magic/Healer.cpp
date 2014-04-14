#include "Healer.h"
#include "../../Engine.h"
#include "../Actor.h"
#include "../Container.h"
#include "../Gui.h"

#include <iostream>

Healer::Healer(const EntityWPtr &owner) 
: Totem::Component<Healer, PropertyUserData>("Healer"), owner(owner)
{
	user_data.entity = owner;
	user_data.component = this;

	amount = owner.lock()->add<float>("Amount", 1.0f);
	message = owner.lock()->add<std::string>("HealerMessage", std::string());

	owner.lock()->registerToEvent1<EntityPtr>("ApplyEffect").connect(this, &Healer::applyEffect);
}

Healer::~Healer()
{
	//std::cout << "Healer is being destroyed!" << std::endl;
}

void Healer::applyEffect(EntityPtr victim) {
	if(victim->hasEvent("Heal",1))
	{
		if( !message.get().empty() )
			Engine::getSingleton()->getGui()->message(TCODColor::lightCyan, message.get().c_str(),victim->getName().c_str(), abs(amount.get()));

		victim->sendEvent1<float>("Heal", amount.get());
	}
}
