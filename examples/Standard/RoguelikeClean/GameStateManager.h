#pragma once

#include <vector>

class IGameState;

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	enum StateID
	{
		STARTUP,
		IDLE,
		NEW_TURN,
		VICTORY,
		DEFEAT,
		STATE_COUNT
	};
	StateID Get() const { return _stateId; }
	void Set(StateID stateId) { _stateId = stateId; }

	void update();

private:
	StateID _stateId;
	std::vector<IGameState*> _state;
};
