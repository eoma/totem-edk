#pragma once

#include "../Components/Player.h"
#include "../Components/Actor.h"
#include "../Components/Map.h"
#include "../Components/Gui.h"
#include "../Components/Pickable.h"
#include "../Entity.h"
#include "../Engine.h"

#include <memory>
#include <iostream>
#include <vector>

class RenderSystem;
typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class RenderSystem : public sigslot::has_slots<>
{
public:
	RenderSystem() : map(nullptr), player(nullptr) {}
	~RenderSystem() { std::cout << "Render System is being destroyed!" << std::endl; }

	void set(Gui *gui)
	{
		this->gui = gui;
		if(gui)
			std::cout << "Set gui for Render System!" << std::endl;
		else
			std::cout << "Removed gui for Render System!" << std::endl;
	}

	void set(Player *player)
	{
		this->player = player;
		if(player)
			std::cout << "Set player for Render System!" << std::endl;
		else
			std::cout << "Removed player for Render System!" << std::endl;
	}

	void set(Map *map)
	{ 
		this->map = map;
		if(map)
			std::cout << "Set map for Render System!" << std::endl;
		else
			std::cout << "Removed map for Render System!" << std::endl;
	}

	void add(Pickable *item)
	{
		for(unsigned int i = 0; i < items.size(); i++)
		{
			if(items[i] == item)
				return;
		}
		items.push_back(item);
		std::cout << "Added item " << item->getOwner()->getName() << " to Render System!" << std::endl;
	}
	void remove(Pickable *item)
	{
		for(unsigned int i = 0; i < items.size(); i++)
		{
			if(items[i] == item)
			{
				//std::cout << "Removed item from Render System!" << std::endl;
				items[i] = items.back();
				items.pop_back();
				return;
			}
		}
	}

	void add(Actor *actor)
	{ 
		for(unsigned int i = 0; i < actors.size(); i++)
		{
			if(actors[i] == actor)
				return;
		}
		actors.push_back(actor); 
		std::cout << "Added actor " << actor->getOwner()->getName() << " to Render System!" << std::endl;
	}
	void remove(Actor *actor)
	{
		for(unsigned int i = 0; i < actors.size(); i++)
		{
			if(actors[i] == actor)
			{
				//std::cout << "Removed actor from Render System!" << std::endl;
				actors[i] = actors.back();
				actors.pop_back();
				return;
			}
		}
		for(unsigned int i = 0; i < corpses.size(); i++)
		{
			if(corpses[i] == actor)
			{
				//std::cout << "Removed corpse from Render System!" << std::endl;
				corpses[i] = corpses.back();
				corpses.pop_back();
				return;
			}
		}
	}

	void add(EntityWPtr corpse)
	{
		auto entity = corpse.lock();
		if(entity->hasComponent<Actor>())
		{
			auto corpseActor = entity->getComponent<Actor>().get();
			remove(corpseActor);
			corpses.push_back(corpseActor);
			std::cout << "Added " << entity->getName() << " to Render System!" << std::endl;
		}
	}

	void render() const 
	{
		if( !map || !player || !gui )
			return;

		map->render();

		for(unsigned int i = 0; i < items.size(); i++)
		{
			auto item = items[i];
			auto item_pos = item->getOwner()->get<Vec2i>("Position").get();
			if(map->isInFov(item_pos))
				item->getOwner()->sendEvent0("Render");
		}

		for(unsigned int i = 0; i < corpses.size(); i++)
		{
			auto corpse = corpses[i];
			if(map->isInFov(corpse->getPosition()))
				corpse->render();
		}

		for(unsigned int i = 0; i < actors.size(); i++)
		{
			auto actor = actors[i];
			if(map->isInFov(actor->getPosition()))
				actor->render();
		}

		// show the player's stats
		//auto engine = Engine::getSingleton();
		//TCODConsole::root->print(1,engine->getScreenHeight()-2, "HP : %d/%d", 
		//	(int)player->getHp(),
		//	(int)player->getMaxHp());

		gui->render();
	}

private:
	std::vector<Actor*> actors;
	std::vector<Actor*> corpses;
	std::vector<Pickable*> items;
	Map *map;
	Player *player;
	Gui *gui;
};