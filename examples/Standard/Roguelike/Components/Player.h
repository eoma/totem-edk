#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include <Totem/PropertyList.h>
#include "../Entity.h"
#include "../Utils/Vec2i.h"

#include <memory>

#include <libtcod.hpp>

class RenderSystem;
typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class Player;
typedef std::shared_ptr<Player> PlayerPtr;

class Player : public Totem::Component<Player, PropertyUserData>
{
public:
	Player(const EntityWPtr &owner, const RenderSystemPtr &system);
    virtual ~Player();

	void checkForInput();
	void handleActionKey(int ascii);

	EntityPtr getOwner() { return owner.lock(); }

	float getMaxHp() const { return maxHp.get(); }
	float getHp() const { return hp.get(); }

	EntityPtr choseFromInventory();

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	RenderSystemPtr system;
	
	bool moveOrAttack(Vec2i target_pos);

	void OnDying();
	void OnDeath();

	Totem::Property<bool> dead;
	Totem::Property<Vec2i> position;

	Totem::Property<float> maxHp; // maximum health points
	Totem::Property<float> hp; // current health points

	Totem::Property<bool> inventoryFull;
	Totem::PropertyList<EntityPtr> inventoryList;
};