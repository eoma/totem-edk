#pragma once

#include "Math/vec2.h"
#include <memory>
#include <vector>

class Entity;
class Actor;
class Skill;
class Attribute;

class ActionManager
{
public:
	ActionManager();
	~ActionManager();

	void add(Actor *actor);
	bool remove(Actor *actor);

	enum MoveTestResult
	{
		RESULT_MOVE,
		RESULT_MOVE_TO_ATTACK,
		RESULT_MOVE_BLOCKED,
		MOVE_TEST_RESULT_COUNT
	};
	MoveTestResult testMove(const std::shared_ptr<Entity> &pawn, const clan::Vec2i &direction);

	enum ActionType
	{
		MOVE,
		ATTACK,
		WAIT,
		ACTION_TYPE_COUNT
	};
	int estimateAction(ActionType action, const std::shared_ptr<Entity> &pawn, const clan::Vec2i &direction);
	void takeAction(ActionType action, const std::shared_ptr<Entity> &pawn, const clan::Vec2i &direction);

private:
	int _handleMoveActionEstimate(const std::shared_ptr<Actor> &actor, const clan::Vec2i &direction);
	int _handleAttackActionEstimate(const std::shared_ptr<Actor> &actor, const clan::Vec2i &direction);

	void _handleMoveAction(const std::shared_ptr<Actor> &actor, const clan::Vec2i &direction);
	void _handleAttackAction(const std::shared_ptr<Actor> &actor, const clan::Vec2i &direction);

	int _calculateElapsedTime(const std::shared_ptr<Skill> &skill, const std::shared_ptr<Attribute> &dexterity);
	int _calculateCooldownTime(const std::shared_ptr<Skill> &skill, const std::shared_ptr<Attribute> &constitution);
	int _calculateAttackDamage(const std::shared_ptr<Skill> &skill, const std::shared_ptr<Attribute> &strength);

	std::shared_ptr<Actor> _find(const std::shared_ptr<Entity> &owner);
	std::vector<Actor*> _actors;
};