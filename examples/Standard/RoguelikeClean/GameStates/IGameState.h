#pragma once

class IGameState
{
public:
	IGameState() {}
	virtual ~IGameState() {}

	virtual void update() = 0;
};