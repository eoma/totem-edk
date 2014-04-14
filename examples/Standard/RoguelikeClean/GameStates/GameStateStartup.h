#pragma once

#include "IGameState.h"

class GameStateStartup : public IGameState
{
public:
	GameStateStartup();
	virtual ~GameStateStartup();

	void update() override;
};