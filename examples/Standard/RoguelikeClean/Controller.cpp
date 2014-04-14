#include "Controller.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "TurnManager.h"
#include "ActionManager.h"
#include "Entity.h"
#include "Components/Actor.h"
#include "Math/vec2.h"
#include <libtcod.hpp>

using namespace clan;

Controller::Controller()
{
	_minions.resize(1, nullptr);
	_allEntities.resize(1, nullptr);
}

Controller::~Controller()
{
}

void Controller::Set(const std::shared_ptr<Entity> &pawn)
{
	if(_minions[0] == nullptr)
	{
		auto minion = std::make_shared<Minion>();
		minion->entity = pawn;
		minion->dir = Vec2i(0);
		minion->actionIntent.resize(ActionManager::ACTION_TYPE_COUNT, false);
		_minions[0] = minion;
		_allEntities[0] = pawn;
	}
	else
	{
		_minions[0]->entity = pawn;
		_allEntities[0] = pawn;
	}
}

void Controller::Add(const std::shared_ptr<Entity> &pawn)
{
	auto minion = std::make_shared<Minion>();
	minion->entity = pawn;
	minion->dir = Vec2i(0);
	minion->actionIntent.resize(ActionManager::ACTION_TYPE_COUNT, false);

	if(_minions[0] == nullptr || _allEntities[0] == nullptr)
	{
		_minions[0] = minion;
		_allEntities[0] = pawn;
	}
	else
	{
		_minions.push_back(minion);
		_allEntities.push_back(pawn);
	}
}

void Controller::think(int elapsedTime)
{
	_resetActionIntent();
	_internalThink(elapsedTime);
}

void Controller::_resetActionIntent()
{
	for(auto minion : _minions)
	{
		if(minion == nullptr)
			continue;

		for(unsigned int i = 0; i < minion->actionIntent.size(); i++)
			minion->actionIntent[i] = false;
	}
}

int Controller::estimateAction(unsigned int index)
{
	int elapsedTime = 0;

	if(index >= _minions.size())
		return elapsedTime;

	auto minion = _minions[index];

	if(minion == nullptr || minion->entity == nullptr)
		return elapsedTime;

	if(minion->actionIntent[ActionManager::MOVE])
		elapsedTime = GameManager::Get()->getAction()->estimateAction(ActionManager::MOVE, minion->entity, minion->dir);
	else if(minion->actionIntent[ActionManager::ATTACK])
		elapsedTime = GameManager::Get()->getAction()->estimateAction(ActionManager::ATTACK, minion->entity, minion->dir);
	else if(minion->actionIntent[ActionManager::WAIT])
		elapsedTime = GameManager::Get()->getAction()->estimateAction(ActionManager::WAIT, minion->entity, minion->dir);
	else
		return elapsedTime;

	return elapsedTime;
}

void Controller::takeAction(unsigned int index)
{
	if(index >= _minions.size())
		return;

	auto minion = _minions[index];

	if(minion == nullptr || minion->entity == nullptr)
		return;

	if(minion->actionIntent[ActionManager::MOVE])
		GameManager::Get()->getAction()->takeAction(ActionManager::MOVE, minion->entity, minion->dir);
	else if(minion->actionIntent[ActionManager::ATTACK])
		GameManager::Get()->getAction()->takeAction(ActionManager::ATTACK, minion->entity, minion->dir);
	
}