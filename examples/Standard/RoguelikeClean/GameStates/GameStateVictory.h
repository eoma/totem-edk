#pragma once

#include "IGameState.h"

class GameStateVictory : public IGameState
{
public:
	GameStateVictory();
	virtual ~GameStateVictory();

	void update() override;
};