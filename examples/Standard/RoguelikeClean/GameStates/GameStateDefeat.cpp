#include "GameStateDefeat.h"
#include "../GameManager.h"

GameStateDefeat::GameStateDefeat()
	: IGameState()
{
}

GameStateDefeat::~GameStateDefeat()
{
}

void GameStateDefeat::update()
{
	GameManager::Get()->terminate();
	GameManager::Get()->restart();
}