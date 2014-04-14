#include "Monster.h"
#include "../Engine.h"
#include "Map.h"
#include "Actor.h"
#include "Gui.h"

#include "../Systems/MonsterSystem.h"

#include <iostream>

Monster::Monster(const EntityWPtr &owner, MonsterFamily family, const MonsterSystemPtr &system) 
: Totem::Component<Monster, PropertyUserData>("Monster"), owner(owner), system(system), family(family)
{
	user_data.entity = owner;
	user_data.component = this;

	dead = owner.lock()->add<bool>("Dead", false);
	position = owner.lock()->add<Vec2i>("Position", Vec2i(0,0));
	level = owner.lock()->add<int>("Level", 1);

	owner.lock()->registerToEvent0("Dying").connect(this, &Monster::OnDying);

	system->add(this, family);
}

Monster::~Monster()
{
	//std::cout << "Monster is being destroyed!" << std::endl;
	system->remove(this);
}

void Monster::update(const float &/*deltaTime*/)
{
	if(dead.get())
		return;

	auto engine = Engine::getSingleton();
	moveOrAttack(engine->getPlayer()->getPosition());
}

void Monster::OnDying()
{
	Engine::getSingleton()->getGui()->message(TCODColor::lightGrey,"The %s is dead",owner.lock()->getName().c_str());
	
	//Stop monster from acting like a monster once it's dead!
	system->remove(this);
}

void Monster::moveOrAttack(const Vec2i &target_pos) {
	auto engine = Engine::getSingleton();
	auto map = engine->getMap();
	static int tdx[8]={-1,0,1,-1,1,-1,0,1};
	static int tdy[8]={-1,-1,-1,0,0,1,1,1};

	if( engine->getPlayer()->isDead() )
	{
		auto rng = TCODRandom::getInstance();
		auto index = rng->getInt(0, 7);
		auto dp = Vec2i(tdx[index], tdy[index]);
		if(map->canWalk(position.get() + dp))
			position += dp;

		return;
	}

	auto dpos = target_pos - position.get();
	auto distance = dpos.length();

	// at melee range
	if(distance < 2)
	{
		if(owner.lock()->hasEvent("Attack", 1))
			owner.lock()->sendEvent1<EntityPtr>("Attack", engine->getPlayer()->getOwner());

		return;
	}
	// if player is not in attack range, but in fov, walk towards him
	else if(map->isInFov(position.get()))
	{
		dpos.normalize();
		auto step = Vec2i(dpos.x() > 0 ? 1:-1, dpos.y() > 0 ? 1:-1);
		auto tp = position.get() + dpos;
		if(map->canWalk(tp)) {
			position += dpos;
			return;
		}
	}

	// player not visible. use scent tracking.
	// find the adjacent cell with the highest scent level
	unsigned int bestLevel=0;
	int bestCellIndex=-1;
	

	for (int i=0; i<8; i++) 
	{
		auto cell = position.get() + Vec2i(tdx[i], tdy[i]);
		if (map->canWalk(cell)) 
		{
			auto cellScent = map->getScent(cell);      
			if (cellScent > map->currentScentValue - SCENT_THRESHOLD && cellScent > bestLevel) 
			{
					bestLevel=cellScent;
					bestCellIndex=i;
			}
		}
	}

	if ( bestCellIndex != -1 ) 
	{
		// the monster smells the player. follow the scent
		position += Vec2i(tdx[bestCellIndex], tdy[bestCellIndex]);
	}
	//if there's no scent, move randomly
	else
	{
		auto rng = TCODRandom::getInstance();
		auto index = rng->getInt(0, 7);
		auto dp = Vec2i(tdx[index], tdy[index]);
		if(map->canWalk(position.get() + dp))
			position += dp;
	}
}
