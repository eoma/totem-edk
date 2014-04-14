#include "GameStateManager.h"

#include "GameStates/GameStateStartup.h"
#include "GameStates/GameStateIdle.h"
#include "GameStates/GameStateNewTurn.h"
#include "GameStates/GameStateVictory.h"
#include "GameStates/GameStateDefeat.h"

GameStateManager::GameStateManager()
	: _stateId(STARTUP)
{
	_state.resize(STATE_COUNT, nullptr);
	_state[STARTUP] = new GameStateStartup();
	_state[IDLE] = new GameStateIdle();
	_state[NEW_TURN] = new GameStateNewTurn();
	_state[VICTORY] = new GameStateVictory();
	_state[DEFEAT] = new GameStateDefeat();
}

GameStateManager::~GameStateManager()
{
	for(auto state : _state)
		delete state;
}

void GameStateManager::update()
{
	_state[_stateId]->update();
}
