#include "Destructible.h"
#include "../Engine.h"
#include "../Systems/RenderSystem.h"

#include <iostream>

Destructible::Destructible(const EntityWPtr &owner, const RenderSystemPtr &system) 
: Totem::Component<Destructible, PropertyUserData>("Destructible"), owner(owner), system(system)
{
	user_data.entity = owner;
	user_data.component = this;

	ch = owner.lock()->add<int>("Character", '@');
	col = owner.lock()->add<TCODColor>("Color", TCODColor::white);
	blocks = owner.lock()->add<bool>("Blocks", true);
	dead = owner.lock()->add<bool>("Dead", false);
	defense = owner.lock()->add<float>("Defense", 1);

	maxHp = owner.lock()->add<float>("MaxHP", 1);
	hp = owner.lock()->add<float>("HP", 1);

	corpseName = owner.lock()->add<std::string>("CorpseName", "corpse");

	owner.lock()->registerToEvent1<float>("TakeDamage").connect(this, &Destructible::takeDamage);
	owner.lock()->registerToEvent1<float>("Heal").connect(this, &Destructible::heal);
}

Destructible::~Destructible()
{
	//std::cout << "Destructible is being destroyed!" << std::endl;
}

void Destructible::takeDamage(float damage) {
    damage -= defense;
    if ( damage > 0 ) {
        hp -= damage;
        if ( hp <= 0 ) {
            die();
        }
    } else {
        damage=0;
    }
}

void Destructible::die() {

	if(owner.lock()->hasEvent("Dying", 0))
		owner.lock()->sendEvent0("Dying");

    // transform the actor into a corpse!
    ch='%';
    col=TCODColor::darkRed;   
	owner.lock()->updateName(corpseName.get());
    blocks=false;

	dead = true;

    // make sure corpses are drawn before living actors
	system->add(owner);
	if(owner.lock()->hasEvent("Dead", 0))
		owner.lock()->sendEvent0("Dead");
}

void Destructible::heal(float amount) 
{
    hp += amount;
    if ( hp.get() > maxHp.get() ) {
        hp=maxHp.get();
    }
	else if ( hp <= 0 ) {
            die();
    }
}

