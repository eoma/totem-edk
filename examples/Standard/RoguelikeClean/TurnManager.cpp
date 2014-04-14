#include "TurnManager.h"
#include "GameManager.h"
#include "ActionManager.h"
#include "MapManager.h"
#include "Player.h"
#include "EntityContainer.h"
#include "Entity.h"
#include "EventDefinitions.h"
#include "PropertyDefinitions.h"
#include "AgentManager.h"

#include "Math/vec2.h"
#include <algorithm>
#include <iostream>

using namespace clan;

TurnManager::TurnManager()
	: _turnCount(0)
{
}

TurnManager::~TurnManager()
{
}

void TurnManager::invoke()
{
	//The player is the one who always initiates a new turn and defines the tick length of that turn.
	int elapsedTime = GameManager::Get()->getPlayer()->estimateAction();

	//Then, all agents are free to think and plan their action(s) within this elapsed time
	GameManager::Get()->getAgent()->think(elapsedTime);

	std::cout << "-----------------------------------" << std::endl;
	std::cout << "Schedule for turn " << _turnCount << " - lasts for " << elapsedTime << (elapsedTime == 1 ? " tick!" : " ticks!") << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	//First, we need to update all components with this new turn's elapsedTime, such that cooldowns, etc, stays up to date with the elapsing time
	for(auto entity : GameManager::Get()->getEntities()->getAll())
		entity->sendEvent1<int>(EVENT_NewTurn, elapsedTime);

	//Then we can run the action schedule
	_runSchedule(elapsedTime);
	
	std::cout << "-----------------------------------" << std::endl;

	//For debugging purposes, write the rest time for all scheduled actions that couldn't complete this turn. They will effectively delay any new
	//actions the next turn.
	for(unsigned int i = 0; i < _schedule.size(); i++)
	{
		auto info = _schedule[i];

		auto pawn = info->controller->Get();
		if(pawn && info->actions.empty() == false)
		{
			auto actionTime = info->actions[0]->CurrentTime;
			std::cout << "- " << pawn->getName() << " has " << actionTime << (actionTime == 1 ? " tick" : " ticks") << " time left on his action after the turn." << std::endl;
		}
	}
	std::cout << "-----------------------------------" << std::endl;

	auto pawn = GameManager::Get()->getPlayer()->Get();
	if(pawn->hasProperty(PROPERTY_POSITION) && pawn->hasProperty(PROPERTY_SIGHT_RADIUS))
	{
		GameManager::Get()->getMap()->computeFov(
			MapManager::LAYER_GROUND, 
			pawn->get<clan::Vec2i>(PROPERTY_POSITION), 
			pawn->get<int>(PROPERTY_SIGHT_RADIUS));
	}

	_turnCount++;
}

void TurnManager::schedule(int time, int actionType, const Vec2i &direction, const std::shared_ptr<Controller> &controller)
{
	auto info = _find(controller);
	if(info == nullptr)
	{
		info = std::make_shared<ScheduleInfo>(controller);
		_schedule.push_back(info);
	}

	info->add(std::make_shared<ActionInfo>(time, actionType, direction));
}

void TurnManager::_runSchedule(int elapsedTime)
{
	_checkForNullInfo();

	//We sort the action schedule based on ticks. The fewer ticks an action takes, the earlier
	//in the schedule the action will run.
	sort( _schedule.begin(), _schedule.end(), TurnManager::ScheduleSorter );

	for(unsigned int i = 0; i < _schedule.size(); i++)
	{
		auto info = _schedule[i];
		if(info->actions.empty())
			continue;

		auto pawn = info->controller->Get();
		if(pawn)
		{
			//First, we need to update all components with this new turn's elapsedTime, such that cooldowns, etc, can do it's work.
			pawn->sendEvent1<int>(EVENT_NewTurn, elapsedTime);

			auto actionTime = info->actions[0]->CurrentTime;
			std::cout << actionTime << (actionTime == 1 ? " tick - " : " ticks - ") << pawn->getName() << std::endl;

			//Let the controller take it's scheduled time. 
			//The restTime is returned, which is how many ticks this controller has left this turn
			auto restTime = _takeAction(info, actionTime, elapsedTime);

			//Check whether the current action is completed
			if(info->actions[0]->CurrentTime <= 0)
				info->actions.erase(info->actions.begin());
			
			//Is there any time left this turn in which this controller can perform an action?
			if(restTime > 0)
			{
				//Check if there's already an action scheduled
				if(info->actions.empty())
				{
					//If no action has been scheduled, then let the controller think of an action to perform within the restTime, 
					//though he is free to schedule an action that will take longer than the restTime, effectively delaying any 
					//actions in the next turn...
					info->controller->think(restTime);
				}

				//Check whether the previous part actually caused the controller to schedule for a new action within this turn.
				if(info->actions.empty())
					continue;

				//If we're here, we simply sort the schedule again, so that the next action of this controller
				//is the one this controller is sorted by, but take into account the previous actionTime, so that it's
				//placed in time correctly in relation to the other scheduled controllers.
				info->actions[0]->CurrentTime += actionTime;

				//Then we have to rerun the scheduler and break out of this one. Mind that all controllers that has already
				//executed their scheduled actions will already have been removed from the list, so this is ok!
				_runSchedule(elapsedTime);
				break;
			}
		}
	}
}

int TurnManager::_takeAction(const std::shared_ptr<ScheduleInfo> &info, int actionTime, int elapsedTime)
{
	//Perform the action for actionTime ticks
	if(info->actions[0]->Activated == false)
	{
		info->controller->takeAction();
		info->actions[0]->Activated = true;
	}

	//Spend the time of this turn
	info->actions[0]->CurrentTime = actionTime - elapsedTime;

	//Return the restTime, indicating how many more ticks this controller has this turn to spend
	return elapsedTime - actionTime;
}

std::shared_ptr<TurnManager::ScheduleInfo> TurnManager::_find(const std::shared_ptr<Controller> &controller)
{
	if(controller)
	{
		for(auto info : _schedule)
		{
			if(info->controller == controller)
				return info;
		}
	}

	return nullptr;
}

void TurnManager::_checkForNullInfo()
{
	for(unsigned int i = 0; i < _schedule.size(); i++)
	{
		if(_schedule[i] == nullptr)
		{
			_schedule.erase(_schedule.begin()+i);
			_checkForNullInfo();
			return;
		}
		else if((_schedule.begin() + i) == _schedule.end())
		{
			_schedule.erase(_schedule.begin()+i);
			_checkForNullInfo();
			return;
		}
		else if(_schedule[i]._Expired())
		{
			_schedule.erase(_schedule.begin()+i);
			_checkForNullInfo();
			return;
		}
	}
}
