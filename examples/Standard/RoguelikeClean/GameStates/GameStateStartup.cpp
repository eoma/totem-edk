#include "GameStateStartup.h"
#include "../GameManager.h"
#include "../Player.h"

GameStateStartup::GameStateStartup()
	: IGameState()
{
}

GameStateStartup::~GameStateStartup()
{
}

void GameStateStartup::update()
{
	GameManager::Get()->getPlayer()->FOV();
}