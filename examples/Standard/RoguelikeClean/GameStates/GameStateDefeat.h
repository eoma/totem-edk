#pragma once

#include "IGameState.h"

class GameStateDefeat : public IGameState
{
public:
	GameStateDefeat();
	virtual ~GameStateDefeat();

	void update() override;
};