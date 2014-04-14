#include "GameStateIdle.h"
#include "../GameManager.h"
#include "../EntityContainer.h"

GameStateIdle::GameStateIdle()
	: IGameState()
{
}

GameStateIdle::~GameStateIdle()
{
}

void GameStateIdle::update()
{
	GameManager::Get()->getEntities()->update();
}