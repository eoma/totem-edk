#include "Pickable.h"
#include "../Engine.h"
#include "Actor.h"
#include "Container.h"

#include "../Systems/RenderSystem.h"

#include <iostream>

Pickable::Pickable(const EntityWPtr &owner, const RenderSystemPtr &system) 
: Totem::Component<Pickable, PropertyUserData>("Pickable"), owner(owner), system(system)
{
	user_data.entity = owner;
	user_data.component = this;

	position = owner.lock()->add<Vec2i>("Position", Vec2i(0,0));
	on_ground = owner.lock()->add<bool>("OnGround", true);

	owner.lock()->registerToEvent1<EntityPtr>("PickUp").connect(this, &Pickable::pick);
	owner.lock()->registerToEvent1<EntityPtr>("Drop").connect(this, &Pickable::drop);

	if(owner.lock()->hasComponent<Actor>())
		system->remove(owner.lock()->getComponent<Actor>().get());

	system->add(this);
}

Pickable::~Pickable()
{
	//std::cout << "Pickable is being destroyed!" << std::endl;
}

void Pickable::pick(EntityPtr wearer) {
	if ( wearer->hasComponent<Container>() && !wearer->get<bool>("InventoryFull").get() ) 
	{
		wearer->sendEvent1<EntityPtr>("PickUp", owner.lock());
		system->remove(this);
		on_ground = false;

		//We still want an item's entity to receive updates to components and properties,
		//so we only remove it's actor component from the world and render system
		if(owner.lock()->hasComponent<Actor>())
			Engine::getSingleton()->remove(owner.lock()->getComponent<Actor>());
    }
}

void Pickable::drop(EntityPtr wearer) {
	
	if(wearer->hasComponent<Container>())
	{
		auto engine = Engine::getSingleton();

		wearer->sendEvent1<EntityPtr>("Remove", owner.lock());

		// Drop item at wearer's feet
		position = wearer->get<Vec2i>("Position").get();
		on_ground = true;

		// Add item back into the world and render system
		if(owner.lock()->hasComponent<Actor>())
			engine->add(owner.lock()->getComponent<Actor>());

		engine->getGui()->message(TCODColor::lightGrey,"%s drops a %s.", wearer->getName().c_str(), owner.lock()->getName().c_str());
	}
}