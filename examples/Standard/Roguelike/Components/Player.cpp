#include "Player.h"
#include "../Engine.h"
#include "Map.h"
#include "Actor.h"
#include "Gui.h"
#include "Pickable.h"
#include "Destructible.h"
#include "../Systems/RenderSystem.h"

#include <iostream>

Player::Player(const EntityWPtr &owner, const RenderSystemPtr &system) 
: Totem::Component<Player, PropertyUserData>("Player"), owner(owner), system(system)
{
	user_data.entity = owner;
	user_data.component = this;

	dead = owner.lock()->add<bool>("Dead", false);
	position = owner.lock()->add<Vec2i>("Position", Vec2i(0,0));
	maxHp = owner.lock()->add<float>("MaxHP", 1);
	hp = owner.lock()->add<float>("HP", 1);
	inventoryFull = owner.lock()->add<bool>("InventoryFull", false);

	inventoryList = owner.lock()->addList<EntityPtr>("Inventory");

	owner.lock()->registerToEvent0("Dying").connect(this, &Player::OnDying);
	owner.lock()->registerToEvent0("Dead").connect(this, &Player::OnDeath);

	system->set(this);
}

Player::~Player()
{
	//std::cout << "Player is being destroyed!" << std::endl;
	system->set(static_cast<Player*>(nullptr));
}

void Player::checkForInput()
{
	auto engine = Engine::getSingleton();
	if(dead.get())
	{
		engine->gameStatus=Engine::NEW_TURN;
		engine->getMap()->getOwner()->sendEvent0("ComputeFOV");
		Sleep(TCODRandom::getInstance()->getInt(100,500));
		return;
	}

	int dx=0,dy=0;
	switch(engine->getLastKey().vk) 
	{
		case TCODK_UP : 
		case TCODK_KP8 :	dy=-1; break;
		case TCODK_KP7 :	dy=-1; dx=-1; break;
		case TCODK_KP9 :	dy=-1; dx=1; break;
		case TCODK_DOWN :	
		case TCODK_KP2 :	dy=1; break;
		case TCODK_KP1 :	dy=1; dx=-1; break;
		case TCODK_KP3 :	dy=1; dx=1; break;
		case TCODK_LEFT :	
		case TCODK_KP4 :	dx=-1; break;
		case TCODK_RIGHT :	
		case TCODK_KP6 :	dx=1; break;
		case TCODK_CHAR : handleActionKey(engine->getLastKey().c); break;
		default:break;
	}

	if ( dx != 0 || dy != 0 ) 
	{
		engine->gameStatus=Engine::NEW_TURN;
		if ( moveOrAttack(position.get() + Vec2i(dx,dy)) ) {
			engine->getMap()->getOwner()->sendEvent0("ComputeFOV");
		}
	}
}

void Player::handleActionKey(int ascii)
{
	auto engine = Engine::getSingleton();
	switch(ascii) 
	{
		case 'g' : // pickup item
		{
			bool found=false;
			auto actors = engine->getActors();
			for (unsigned int i = 0; i < actors.size(); i++) 
			{
				auto actor = actors[i];
				if ( actor->getOwner()->hasComponent<Pickable>() && actor->getPosition() == position.get() ) 
				{
					if(inventoryFull.get())
					{
						found = true;
						engine->getGui()->message(TCODColor::red,"Your inventory is full.");
					}
					else
					{
						actor->getOwner()->sendEvent1<EntityPtr>("PickUp", owner.lock());
						engine->getGui()->message(TCODColor::lightGrey,"You pick up the %s.", actor->getOwner()->getName().c_str());
						engine->gameStatus=Engine::NEW_TURN; //Spend one turn for picking up one item
						found = true;
					}
					break;
				}
			}
			if (!found) 
				engine->getGui()->message(TCODColor::lightGrey,"There's nothing here to get.");
		} break;
		case 'i' : // display inventory
		{
			auto item = choseFromInventory();
			if(item)
			{
				item->sendEvent1<EntityPtr>("Use", owner.lock());
				engine->gameStatus = Engine::NEW_TURN;
			}
		} break;
		case 'd' : // drop item
		{
			auto item = choseFromInventory();
			if(item)
			{
				item->sendEvent1<EntityPtr>("Drop", owner.lock());
				engine->gameStatus = Engine::NEW_TURN;
			}
		} break;
	};
}

EntityPtr Player::choseFromInventory()
{
	static const int INVENTORY_WIDTH=50;
    static const int INVENTORY_HEIGHT=28;
    static TCODConsole con(INVENTORY_WIDTH,INVENTORY_HEIGHT);

	// display the inventory frame
	con.setDefaultForeground(TCODColor(200,180,50));
	con.printFrame(0,0,INVENTORY_WIDTH,INVENTORY_HEIGHT,true,TCOD_BKGND_DEFAULT,"inventory");

	// display the items with their keyboard shortcut
	con.setDefaultForeground(TCODColor::white);
	int shortcut='a';
	int y=1;
	auto inventory = inventoryList.get();
	for (unsigned int i = 0; i < inventory.size(); i++) 
	{
		auto item = inventory[i];
		con.print(2,y,"(%c) %s", shortcut, item->getName().c_str());
		y++;
		shortcut++;
	}

	// blit the inventory console on the root console
	auto engine = Engine::getSingleton();
	TCODConsole::blit(	&con, 0,0,INVENTORY_WIDTH,INVENTORY_HEIGHT, TCODConsole::root, 
						engine->getScreenWidth()/2 - INVENTORY_WIDTH/2, 
						engine->getScreenHeight()/2-INVENTORY_HEIGHT/2	);
	TCODConsole::flush();

	// wait for a key press
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
	if ( key.vk == TCODK_CHAR ) 
	{
		int itemIndex=key.c - 'a';
		if ( itemIndex >= 0 && itemIndex < inventory.size() ) 
		{
			return inventory[itemIndex];
		}
	}
	return nullptr;
}

void Player::OnDying()
{
	Engine::getSingleton()->getGui()->message(TCODColor::red,"You died!");
}

void Player::OnDeath()
{
	Engine::getSingleton()->gameStatus = Engine::DEFEAT;
}

bool Player::moveOrAttack(Vec2i target_pos) {
	auto engine = Engine::getSingleton();
    if ( engine->getMap()->isWall(target_pos) ) 
		return false;

    // look for living actors to attack
	auto actors = engine->getActors();
    for (unsigned int i = 0; i < actors.size(); i++) {
        auto actor = actors[i];
		if (	(actor->getOwner()->hasComponent<Destructible>() && !actor->getOwner()->get<bool>("Dead").get()) 
				&& actor->getPosition() == target_pos )
		{
			owner.lock()->sendEvent1<EntityPtr>("Attack", actor->getOwner());
            return false;
        }
    }

	// look for corpses or items
	for (unsigned int i = 0; i < actors.size(); i++) {
        auto actor = actors[i];
		if ( (	(actor->getOwner()->hasComponent<Destructible>() && actor->getOwner()->get<bool>("Dead").get()) 
				|| actor->getOwner()->hasComponent<Pickable>() ) 
				&& actor->getPosition() == target_pos ) 
		{
			Engine::getSingleton()->getGui()->message(TCODColor::lightGrey,"There's a %s here.",actor->getOwner()->getName().c_str());
		}
	}

	position = target_pos;
    return true;
}

