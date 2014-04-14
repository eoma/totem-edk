#pragma once

#include <memory>
#include <string>

class Parser;
class GameStateManager;
class TurnManager;
class RenderManager;
class ActionManager;
class MapManager;
class AgentManager;
class Player;
class EntityContainer;
class EntityFactory;

class GameManager
{
public:
	~GameManager();
	static std::shared_ptr<GameManager> Get();
	static void Shutdown();

private:

#ifdef MSVC
	#if(_MSC_VER >= 1700)
	friend class std::_Ref_count_obj<GameManager>;
	#else
	friend class std::tr1::_Ref_count_obj<GameManager>;
	#endif
#endif

	GameManager();
	static std::shared_ptr<GameManager> _instance;

public:
	bool initialize(const std::string &resourceDir);
	void run();
	void restart();
	void terminate();
	void close();

public:
	const std::shared_ptr<GameStateManager> &getState() { return _state; }
	const std::shared_ptr<TurnManager> &getTurn() { return _turn; }
	const std::shared_ptr<RenderManager> &getRender() { return _renderer; }
	const std::shared_ptr<ActionManager> &getAction() { return _action; }
	const std::shared_ptr<MapManager> &getMap() { return _map; }
	const std::shared_ptr<AgentManager> &getAgent() { return _agent; }
	const std::shared_ptr<Player> &getPlayer() { return _player; }
	const std::shared_ptr<EntityContainer> &getEntities() { return _entities; }
	const std::shared_ptr<EntityFactory> &getFactory() { return _factory; }

private:
	void _load();
	void _save();

	void _update();
	void _render();

private:
	std::unique_ptr<Parser> _parser;

	std::shared_ptr<GameStateManager> _state;
	std::shared_ptr<TurnManager> _turn;
	std::shared_ptr<RenderManager> _renderer;
	std::shared_ptr<ActionManager> _action;
	std::shared_ptr<MapManager> _map;
	std::shared_ptr<AgentManager> _agent;
	std::shared_ptr<Player> _player;
	std::shared_ptr<EntityContainer> _entities;
	std::shared_ptr<EntityFactory> _factory;

	bool running;
};
