#include "Engine.h"

#include "Entity.h"
#include "EntityParser.h"
#include "Systems/PrefabSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/MonsterSystem.h"
#include "Utils/Vec2i.h"
#include "Components/Actor.h"
#include "Components/Map.h"
#include "Components/Destructible.h"
#include "Components/Attacker.h"
#include "Components/Player.h"
#include "Components/Monster.h"
#include "Components/Gui.h"
#include "Components/Container.h"

EnginePtr Engine::singleton;

EnginePtr Engine::getSingleton()
{
	if(singleton == nullptr)
		singleton = std::make_shared<Engine>();
	return singleton;
}

void Engine::Shutdown()
{
	if(singleton)
		singleton.reset();
}

Engine::Engine()
	: fovRadius(10), gameStatus(STARTUP), screenWidth(0), screenHeight(screenHeight)
{
}

Engine::~Engine()
{
}

void Engine::init(const std::string &resource_dir, int screenWidth, int screenHeight)
{
	this->resource_dir = resource_dir;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	TCODConsole::setCustomFont((resource_dir+"terminal.png").c_str());
	TCODConsole::initRoot(screenWidth,screenHeight, "Totem Roguelike", false);

	prefab_system = std::make_shared<PrefabSystem>();
	render_system = std::make_shared<RenderSystem>();
	monster_system = std::make_shared<MonsterSystem>();
	entity_parser = std::make_shared<EntityParser>();
	std::cout << "Engine init done!" << std::endl << std::endl;
}

void Engine::restart()
{
	std::cout << "Engine restarting!" << std::endl;
	auto guiEntity = std::make_shared<Entity>("Gui");
	gui = guiEntity->addComponent( std::make_shared<Gui>(guiEntity, render_system) );
	entities.push_back(guiEntity);

	auto playerEntity = prefab_system->instantiate("player");
	player = playerEntity->getComponent<Actor>();
	player_input = playerEntity->getComponent<Player>();

	auto mapEntity = std::make_shared<Entity>("Map");
	map = mapEntity->addComponent( std::make_shared<Map>(mapEntity, screenWidth,screenHeight - PANEL_HEIGHT, render_system) );
	map->init(true);
	entities.push_back(mapEntity);

	gui->message(TCODColor::red, "Welcome stranger!\nPrepare to perish in the Tombs of the Ancient Kings.");
	gameStatus = STARTUP;
	std::cout << "Engine restart done!" << std::endl << std::endl;
}

void Engine::terminate()
{
	std::cout << "Engine terminating!" << std::endl;
	gui.reset();
	player.reset();
	player_input.reset();
	map.reset();
	while( !entities.empty() )
	{
		remove(entities[0]);
	}
	std::cout << "Engine termination done!" << std::endl << std::endl;
}

void Engine::update()
{
	if ( gameStatus == STARTUP ) 
		map->computeFov();

	else if(gameStatus == DEFEAT)
	{
		terminate();
		restart();
		return;
	}

	gameStatus=IDLE;

    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&last_key,&mouse);
	player_input->checkForInput();

	if(gameStatus == NEW_TURN)
	{
		if(player->isAlive())
			map->currentScentValue++;

		for(unsigned int i = 0; i < entities.size(); i++)
		{
			entities[i]->updateComponents(1);
			entities[i]->updateProperties();
		}

		//Update monsters' decissions after everything else, so that
		//they can react to environmental changes, etc.
		monster_system->think();
	}
}

void Engine::render()
{
	TCODConsole::root->clear();
	render_system->render();
}

EntityPtr Engine::createActor(const std::string &name, const Vec2i &pos, int characterID, TCODColor color)
{
	auto actor = std::make_shared<Entity>(name);
	auto actor_ptr = actor->addComponent( std::make_shared<Actor>(actor, render_system) );
	actor->get<Vec2i>("Position") = pos;
	actor->get<int>("Character") = characterID;
	actor->get<TCODColor>("Color") = color;
	actors.push_back(actor_ptr);
	entities.push_back(actor);
	return actor;
}

EntityPtr Engine::createMonster(EntityPtr actor, const std::string &corpse_name, float defense, float maxHp, float power)
{
	if( !actor->hasComponent<Actor>() )
		return nullptr; //If someone wants to be stupid, then act stupid back...

	actor->addComponent( std::make_shared<Destructible>(actor, render_system) );
	actor->addComponent( std::make_shared<Monster>(actor, GOBLIN_FAMILY, monster_system) );
	actor->addComponent( std::make_shared<Attacker>(actor) );
	actor->get<float>("Defense") = defense;
	actor->get<float>("MaxHP") = maxHp;
	actor->get<float>("HP") = maxHp;
	actor->get<std::string>("CorpseName") = corpse_name;
	actor->get<float>("Power") = power;
	return actor;
}

void Engine::remove(const EntityPtr &entity)
{
	if(entity->hasComponent<Actor>())
		remove(entity->getComponent<Actor>());

	if(entity->hasComponent<Pickable>())
		remove(entity->getComponent<Pickable>());

	if(entity->hasComponent<Monster>())
		remove(entity->getComponent<Monster>());

	for(unsigned int i = 0; i < entities.size(); i++)
	{
		if(entities[i] == entity)
		{
			entities[i] = entities.back();
			entities.pop_back();
			return;
		}
	}
}

void Engine::remove(const ActorPtr &actor)
{
	render_system->remove(actor.get());

	for(unsigned int i = 0; i < actors.size(); i++)
	{
		if(actors[i] == actor)
		{
			actors[i] = actors.back();
			actors.pop_back();
			return;
		}
	}
}

void Engine::remove(const PickablePtr &pickable)
{
	render_system->remove(pickable.get());
}

void Engine::remove(const MonsterPtr &monster)
{
	monster_system->remove(monster.get());
}

void Engine::add(const EntityPtr &entity)
{
	entities.push_back(entity);

	if(entity->hasComponent<Actor>())
		add(entity->getComponent<Actor>());
}

void Engine::add(const ActorPtr &actor)
{
	render_system->add(actor.get());
	actors.push_back(actor);
}

ActorPtr Engine::getClosestMonster(const Vec2i &pos, float range) const
{
	ActorPtr closest = nullptr;
	float bestDistance=1E6f;

	for(unsigned int i = 0; i < actors.size(); i++)
	{
		auto actor = actors[i];
		if(pos == actor->getPosition() && actor == player)
			continue;

		if(actor->isAlive())
		{
			auto distance = actor->getPosition().distancef(pos);
			if(distance < bestDistance && (distance <= range || range == 0.0f))
			{
				bestDistance = distance;
				closest = actor;
			}
		}
	}
	return closest;
}

bool Engine::pickATile(Vec2i &pos, float maxRange)
{
	while( !TCODConsole::isWindowClosed() )
	{
		render();
		// highlight the possible range
		for (int cx=0; cx < map->getWidth(); cx++) 
		{
			for (int cy=0; cy < map->getHeight(); cy++) 
			{
				auto cp = Vec2i(cx, cy);
				if ( map->isInFov(cp) && ( maxRange == 0 || cp.distancef(player->getPosition()) <= maxRange) ) 
				{
					TCODColor col=TCODConsole::root->getCharBackground(cx,cy);
					col = col * 1.2f;
					TCODConsole::root->setCharBackground(cx,cy,col);
				}
			}
		}

		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&last_key,&mouse);
		Vec2i mouse_pos = Vec2i(mouse.cx,mouse.cy);
		if ( map->isInFov(mouse_pos) && ( maxRange == 0 || mouse_pos.distancef(player->getPosition()) <= maxRange )) 
		{
			TCODConsole::root->setCharBackground(mouse.cx,mouse.cy,TCODColor::white);
			if ( mouse.lbutton_pressed ) 
			{
				pos = mouse_pos;
				return true;
			}
		}

		if (mouse.rbutton_pressed || last_key.vk != TCODK_NONE)
			return false;

		TCODConsole::flush();
	}
	return false;
}

ActorPtr Engine::getActor(const Vec2i &pos) const
{
	for(unsigned int i = 0; i < actors.size(); i++)
	{
		auto actor = actors[i];
		if(actor->getPosition() == pos && actor->isAlive())
			return actor;
	}
	return nullptr;
}

void Engine::save()
{
	std::cout << "Engine saves session!" << std::endl;
	std::string path = resource_dir + "game.sav";
	if( player->isDead() )
		TCODSystem::deleteFile(path.c_str());
	else
	{
		TCODZip zip;

		//Save gui log
		gui->getOwner()->save(zip);
		gui->save(zip);

		//Save player data
		player->getOwner()->save(zip);

		//Save map data
		zip.putInt(map->getWidth());
		zip.putInt(map->getHeight());
		map->getOwner()->save(zip);
		map->save(zip);

		//Store number of entities, minus map, player and gui
		zip.putInt(entities.size()-3);
		for(unsigned int i = 0; i < entities.size(); i++)
		{
			auto entity = entities[i];

			//If it's a map, a player, a gui, or it has the property OnGround, but it's not on the ground, then we ignore it
			//since it's serialization is handled elsewhere...
			if(entity->hasComponent<Map>() || entity->hasComponent<Player>() || entity->hasComponent<Gui>() ||
				(entity->hasProperty("OnGround") && entity->get<bool>("OnGround") == false))
				continue;
			zip.putString(entity->getName().c_str());
			entity->save(zip);
		}
		
		zip.saveToFile(path.c_str());
	}

	std::cout << "Engine save done!" << std::endl << std::endl;
}

void Engine::load()
{
	std::string path = resource_dir + "game.sav";
	if( TCODSystem::fileExists(path.c_str()) )
	{
		std::cout << "Engine loads savegame!" << std::endl;
		TCODZip zip;
		zip.loadFromFile(path.c_str());

		//Load gui
		auto guiEntity = std::make_shared<Entity>("Gui");
		gui = guiEntity->addComponent( std::make_shared<Gui>(guiEntity, render_system) );
		guiEntity->load(zip);
		gui->load(zip);
		entities.push_back(guiEntity);

		//Load player
		auto playerEntity = prefab_system->instantiate("player");
		player = playerEntity->getComponent<Actor>();
		player_input = playerEntity->getComponent<Player>();
		playerEntity->load(zip);

		//Load map data
		int width = zip.getInt();
		int height = zip.getInt();
		
		auto mapEntity = std::make_shared<Entity>("Map");
		map = mapEntity->addComponent( std::make_shared<Map>(mapEntity, width,height, render_system) );
		mapEntity->load(zip);
		map->load(zip);
		entities.push_back(mapEntity);

		int num_entities = zip.getInt();
		for(int i = 0; i < num_entities; i++)
		{
			std::cout << "Bytes remaining: " << zip.getRemainingBytes() << std::endl;
			if(zip.getRemainingBytes() <= 4)
			{
				std::cout << "ERROR! Failed to load from save file. Terminating and starting a fresh game!" << std::endl << std::endl;
				TCODSystem::deleteFile(path.c_str());
				terminate();
				restart();
				return;
			}

			std::string entity_name = zip.getString();
			auto entity = prefab_system->instantiate(entity_name);
			if(entity)
			{
				entity->load(zip);
				//entities.push_back(entity);
			}
			else
			{
				std::cout << "ERROR! Failed to load from save file. Terminating and starting a fresh game!" << std::endl << std::endl;
				TCODSystem::deleteFile(path.c_str());
				terminate();
				restart();
				return;
			}
		}
		std::cout << "Engine loading done!" << std::endl << std::endl;
	}
	else
	{
		restart();
	}
}
