#include "Player.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "TurnManager.h"
#include "ActionManager.h"
#include "MapManager.h"
#include "Entity.h"
#include "PropertyDefinitions.h"
#include "Components/Actor.h"
#include "Math/vec2.h"
#include <libtcod.hpp>

using namespace clan;

Player::Player()
	: Controller()
{
}

Player::~Player()
{
}

void Player::FOV()
{
	if(Get() && Get()->hasProperty(PROPERTY_POSITION) && Get()->hasProperty(PROPERTY_SIGHT_RADIUS))
	{
		GameManager::Get()->getMap()->computeFov(
			MapManager::LAYER_GROUND, 
			Get()->get<Vec2i>(PROPERTY_POSITION), 
			Get()->get<int>(PROPERTY_SIGHT_RADIUS));
	}
}

void Player::_internalThink(int elapsedTime)
{
	_handleInput();
}

void Player::_handleInput()
{
	TCOD_key_t key;
	TCOD_mouse_t mouse;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE,&key,&mouse);

	if(key.vk == TCODK_ESCAPE)
	{
		GameManager::Get()->close();
		return;
	}

	if(Get() == nullptr)
		return;

	bool waitTurn = false;
	Vec2i dir;
	switch(key.vk) 
	{
		case TCODK_UP : 
		case TCODK_KP8 :	dir.y=-1; break;
		case TCODK_KP7 :	dir.y=-1; dir.x=-1; break;
		case TCODK_KP9 :	dir.y=-1; dir.x=1; break;
		case TCODK_DOWN :	
		case TCODK_KP2 :	dir.y=1; break;
		case TCODK_KP1 :	dir.y=1; dir.x=-1; break;
		case TCODK_KP3 :	dir.y=1; dir.x=1; break;
		case TCODK_LEFT :	
		case TCODK_KP4 :	dir.x=-1; break;
		case TCODK_RIGHT :	
		case TCODK_KP6 :	dir.x=1; break;
		case TCODK_KP5 :	waitTurn = true;
		case TCODK_CHAR : _handleActionKeyInput(key.c); break;
		default:break;
	}

	auto minion = _minions[0];
	if(minion == nullptr)
		return;

	if( dir.x != 0 || dir.y != 0)
	{
		
		minion->dir = dir;
		minion->dir.x = clan::clamp<int, int, int>(minion->dir.x, -1, 1);
		minion->dir.y = clan::clamp<int, int, int>(minion->dir.y, -1, 1);

		auto result = GameManager::Get()->getAction()->testMove(Get(), minion->dir);
		switch(result)
		{
		case ActionManager::RESULT_MOVE:
		case ActionManager::RESULT_MOVE_TO_ATTACK:
			minion->currentIntent = (ActionManager::ActionType)result;
			minion->actionIntent[result] = true;
			GameManager::Get()->getState()->Set(GameStateManager::NEW_TURN);
			break;

		case ActionManager::RESULT_MOVE_BLOCKED:
		default:
			break;
		};
	}
	else if(waitTurn)
	{
		minion->currentIntent = ActionManager::WAIT;
		minion->actionIntent[ActionManager::WAIT] = true;
		GameManager::Get()->getState()->Set(GameStateManager::NEW_TURN);
	}
}

void Player::_handleActionKeyInput(int ascii)
{
}

int Player::estimateAction(unsigned int index)
{
	int elapsedTime = Controller::estimateAction(index);
	auto minion = _minions[0];
	if(minion)
		GameManager::Get()->getTurn()->schedule(elapsedTime, minion->currentIntent, minion->dir, shared_from_this());
	return elapsedTime;
}

void Player::takeAction(unsigned int index)
{
	Controller::takeAction(index);
}
