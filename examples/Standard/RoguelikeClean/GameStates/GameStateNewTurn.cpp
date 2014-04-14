#include "GameStateNewTurn.h"
#include "../GameManager.h"
#include "../TurnManager.h"

GameStateNewTurn::GameStateNewTurn()
	: IGameState()
{
}

GameStateNewTurn::~GameStateNewTurn()
{
}

void GameStateNewTurn::update()
{
	GameManager::Get()->getTurn()->invoke();
}