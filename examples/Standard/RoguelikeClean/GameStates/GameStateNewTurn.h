#pragma once

#include "IGameState.h"

class GameStateNewTurn : public IGameState
{
public:
	GameStateNewTurn();
	virtual ~GameStateNewTurn();

	void update() override;
};