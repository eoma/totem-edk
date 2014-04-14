#include "Agent.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "TurnManager.h"
#include "ActionManager.h"
#include "MapManager.h"
#include "Entity.h"
#include "PropertyDefinitions.h"
#include "Components/Actor.h"
#include "Player.h"
#include "Math/vec2.h"
#include <libtcod.hpp>

using namespace clan;

Agent::Agent()
	: Controller()
{
}

Agent::~Agent()
{
}

void Agent::_internalThink(int elapsedTime)
{
	if(Get() == nullptr)
		return;

	auto minion = _minions[0];
	if(minion == nullptr || Get()->hasProperty(PROPERTY_POSITION) == false)
		return;

	auto player = GameManager::Get()->getPlayer()->Get();
	if(player == nullptr || player->hasProperty(PROPERTY_POSITION) == false)
		return;

	auto position = Get()->get<Vec2i>(PROPERTY_POSITION).get();
	auto playerPosition = player->get<Vec2i>(PROPERTY_POSITION).get();

	//If we are out of view, only perform an action 1/3 of the time
	if(GameManager::Get()->getMap()->isInFov(MapManager::LAYER_GROUND, position) == false)
	{
		auto rng = TCODRandom::getInstance();
		if(rng->getInt(0,3) > 1)
			return;
	}

	//TODO: As we get more actions to perform, this shouldn't only try to move/attack towards the player,
	//but be a bit more intelligent :-P
	int elapsedTimeOfAction = 0;
	auto dir = playerPosition - position;
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
			break;

		case ActionManager::RESULT_MOVE_BLOCKED:
		default:
			break;
		};

		elapsedTimeOfAction = estimateAction();
		/*if(elapsedTimeOfAction > elapsedTime)
		{
			minion->actionIntent[ActionManager::WAIT] = true;
			elapsedTimeOfAction = estimateAction();
			GameManager::Get()->getTurn()->schedule(elapsedTimeOfAction, ActionManager::WAIT, minion->dir, shared_from_this());
		}
		else
		{*/
			GameManager::Get()->getTurn()->schedule(elapsedTimeOfAction, result, minion->dir, shared_from_this());
			
			//Check if we have time to perform the action again within this turn
			/*auto restOfElapsedTime = elapsedTime - elapsedTimeOfAction;
			if(restOfElapsedTime > 0)
				GameManager::Get()->getTurn()->schedule(elapsedTimeOfAction, shared_from_this());*/
		//}
	}
	else
	{
		minion->currentIntent = ActionManager::WAIT;
		minion->actionIntent[ActionManager::WAIT] = true;
		elapsedTimeOfAction = estimateAction();
		GameManager::Get()->getTurn()->schedule(elapsedTimeOfAction, ActionManager::WAIT, minion->dir, shared_from_this());
	}
}

int Agent::estimateAction(unsigned int index)
{
	int elapsedTime = Controller::estimateAction(index);
	return elapsedTime;
}

void Agent::takeAction(unsigned int index)
{
	Controller::takeAction(index);
}
