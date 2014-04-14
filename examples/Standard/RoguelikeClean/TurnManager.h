#pragma once

#include "Entity.h"

#include "Math/vec2.h"
#include <memory>
#include <vector>

class Controller;
class EntityContainer;
class Skill;

class TurnManager
{
public:
	TurnManager();
	~TurnManager();

	void invoke();
	void schedule(int time, int actionType, const clan::Vec2i &direction, const std::shared_ptr<Controller> &controller);

private:
	unsigned long _turnCount;

	struct ActionInfo
	{
		bool Activated;
		int CurrentTime;
		int ActionType;
		clan::Vec2i Direction;

		ActionInfo(int time, int actionType, const clan::Vec2i &direction)
			: Activated(false), CurrentTime(time), ActionType(actionType), Direction(direction)
		{
		}
	};

	struct ScheduleInfo
	{
		std::vector<std::shared_ptr<ActionInfo>> actions;
		std::shared_ptr<Controller> controller;
		void add(const std::shared_ptr<ActionInfo> &action) 
		{ 
			actions.push_back(action); 
		}
		ScheduleInfo(const std::shared_ptr<Controller> &controller) : controller(controller) {}
	};

	std::vector<std::shared_ptr<ScheduleInfo>> _schedule;

	void _runSchedule(int elapsedTime);
	int _takeAction(const std::shared_ptr<ScheduleInfo> &info, int actionTime, int elapsedTime);
	std::shared_ptr<ScheduleInfo> _find(const std::shared_ptr<Controller> &controller);
	void _checkForNullInfo();

	static bool ScheduleSorter(std::shared_ptr<ScheduleInfo> a, std::shared_ptr<ScheduleInfo> b)
	{
		if(a == nullptr && b == nullptr)
			return true;
		else if(a == nullptr)
			return false;
		else if(b == nullptr)
			return true;

		if(a->actions.empty() && b->actions.empty())
			return true;
		else if(a->actions.empty())
			return false;
		else if(b->actions.empty())
			return true;
		else
			return a->actions[0]->CurrentTime < b->actions[0]->CurrentTime;
	}
};
