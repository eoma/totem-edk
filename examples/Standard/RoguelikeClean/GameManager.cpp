#include "GameManager.h"
#include "Parser.h"
#include "GameStateManager.h"
#include "TurnManager.h"
#include "RenderManager.h"
#include "ActionManager.h"
#include "MapManager.h"
#include "AgentManager.h"
#include "EntityContainer.h"
#include "EntityFactory.h"
#include "EntityDefinitions.h"
#include "Player.h"
#include <libtcod.hpp>

std::shared_ptr<GameManager> GameManager::_instance;

std::shared_ptr<GameManager> GameManager::Get()
{
	if(_instance == nullptr)
		_instance = std::make_shared<GameManager>();

	return _instance;
}

void GameManager::Shutdown()
{
	if(_instance != nullptr)
	{
		_instance->terminate();
		_instance.reset();
	}
}

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

bool GameManager::initialize(const std::string &resourceDir)
{
	_parser = std::unique_ptr<Parser>(new Parser(resourceDir));
	_state = std::make_shared<GameStateManager>();
	_turn = std::make_shared<TurnManager>();
	_renderer = std::make_shared<RenderManager>();
	_map = std::make_shared<MapManager>();
	_action = std::make_shared<ActionManager>();
	_agent = std::make_shared<AgentManager>();
	_player = std::make_shared<Player>();
	_entities = std::make_shared<EntityContainer>();
	_factory = std::make_shared<EntityFactory>();

	auto cfg = _parser->parseCfg("Config.cfg");
	TCODConsole::setCustomFont((resourceDir + cfg.Font).c_str());
	TCODConsole::initRoot(cfg.Width, cfg.Height, cfg.Title.c_str(), cfg.Fullscreen);

	//TODO: Might want to add a loading screen here?

	_renderer->initialize();

	if( !_factory->initialize(_entities, _parser->parseStats("Stats.cfg"), _parser->parseEntities("Entities.cfg")) )
		return false;

	_player->Set(_factory->instantiate(ENTITY_HUMAN));

	_map->initialize(_parser->parseMaps("Maps.cfg"));
	_map->generate();
	_map->generateEnemies();

	return true;
}

void GameManager::run()
{
	_load();

	running = true;
	while( TCODConsole::isWindowClosed() == false && running == true )
	{
		_update();
		_render();
	}

	_save();
}

void GameManager::restart()
{
	_state->Set(GameStateManager::STARTUP);
}

void GameManager::terminate()
{
	_player->Set(nullptr);
	_entities->clear();
}

void GameManager::close()
{
	running = false;
}

void GameManager::_load()
{
}

void GameManager::_save()
{
}

void GameManager::_update()
{
	_state->update();
	_state->Set(GameStateManager::IDLE);

	_player->think();
}

void GameManager::_render()
{
	TCODConsole::root->clear();
	_renderer->render();
	TCODConsole::flush();
}
