#pragma once

#include "IGameState.h"

class GameStateIdle : public IGameState
{
public:
	GameStateIdle();
	virtual ~GameStateIdle();

	void update() override;
};