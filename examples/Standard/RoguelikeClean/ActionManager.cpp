#include "ActionManager.h"
#include "GameManager.h"
#include "MapManager.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "Components/Actor.h"
#include "PropertyDefinitions.h"
#include "Stats/StatDefinitions.h"

using namespace clan;

ActionManager::ActionManager()
{
}

ActionManager::~ActionManager()
{
}

void ActionManager::add(Actor *actor)
{
	auto it = std::find(_actors.begin(), _actors.end(), actor);
	if(it == _actors.end())
		_actors.push_back(actor);
}

bool ActionManager::remove(Actor *actor)
{
	auto it = std::find(_actors.begin(), _actors.end(), actor);
	if(it != _actors.end())
	{
		_actors.erase(it);
		return true;
	}

	return false;
}

ActionManager::MoveTestResult ActionManager::testMove(const std::shared_ptr<Entity> &pawn, const clan::Vec2i &direction)
{
	if(pawn->hasProperty(PROPERTY_POSITION) == false)
		return RESULT_MOVE_BLOCKED;

	auto position = pawn->get<Vec2i>(PROPERTY_POSITION).get();
	auto destination = position + direction;

	if(GameManager::Get()->getMap()->isWall(MapManager::LAYER_GROUND, destination))
		return RESULT_MOVE_BLOCKED;

	for(auto actor : _actors)
	{
		if(actor->Owner() == pawn.get())
		{
			auto walk = actor->GetSkill(SKILL_WALK);
			if(walk == nullptr || walk->isOnCooldown())
				return RESULT_MOVE_BLOCKED;

			continue;
		}

		if(actor->Owner()->hasProperty(PROPERTY_POSITION) == false)
			continue;

		auto actorPosition = actor->Owner()->get<Vec2i>(PROPERTY_POSITION).get();
		if(actorPosition == destination)
			return RESULT_MOVE_TO_ATTACK;
	}

	return RESULT_MOVE;
}

int ActionManager::estimateAction(ActionManager::ActionType action, const std::shared_ptr<Entity> &pawn, const clan::Vec2i &direction)
{
	int elapsedTime = 0;

	auto actor = _find(pawn);
	if(actor == nullptr)
		return elapsedTime;

	switch(action)
	{
	case MOVE:
		elapsedTime = _handleMoveActionEstimate(actor, direction);
		break;
	case ATTACK:
		elapsedTime = _handleAttackActionEstimate(actor, direction);
		break;
	case WAIT:
		elapsedTime = 1;
		break;
	default:break;
	};

	return elapsedTime;
}

int ActionManager::_handleMoveActionEstimate(const std::shared_ptr<Actor> &actor, const clan::Vec2i &/*direction*/)
{
	int elapsedTime = 0;
	if(actor->Owner()->hasProperty(PROPERTY_POSITION) == false)
		return elapsedTime;

	auto walk = actor->GetSkill(SKILL_WALK);
	if(walk == nullptr)
		return elapsedTime;

	auto dexterity = actor->GetAttribute(ATTRIBUTE_DEXTERITY);
	if(dexterity == nullptr)
		return elapsedTime;

	elapsedTime = _calculateElapsedTime(walk, dexterity);

	return elapsedTime;
}

int ActionManager::_handleAttackActionEstimate(const std::shared_ptr<Actor> &actor, const clan::Vec2i &direction)
{
	int elapsedTime = 0;
	if(actor->Owner()->hasProperty(PROPERTY_POSITION) == false)
		return elapsedTime;

	auto hit = actor->GetSkill(SKILL_HIT);
	if(hit == nullptr)
		return elapsedTime;

	auto dexterity = actor->GetAttribute(ATTRIBUTE_DEXTERITY);
	if(dexterity == nullptr)
		return elapsedTime;

	elapsedTime = _calculateElapsedTime(hit, dexterity);

	return elapsedTime;
}

void ActionManager::takeAction(ActionManager::ActionType action, const std::shared_ptr<Entity> &pawn, const clan::Vec2i &direction)
{
	auto actor = _find(pawn);
	if(actor == nullptr)
		return;

	switch(action)
	{
	case MOVE:
		_handleMoveAction(actor, direction);
		break;
	case ATTACK:
		_handleAttackAction(actor, direction);
		break;
	case WAIT:
		break;
	default:break;
	};
}

void ActionManager::_handleMoveAction(const std::shared_ptr<Actor> &actor, const clan::Vec2i &direction)
{
	if(actor->Owner()->hasProperty(PROPERTY_POSITION) == false)
		return;

	auto walk = actor->GetSkill(SKILL_WALK);
	if(walk == nullptr)
		return;

	auto constitution = actor->GetAttribute(ATTRIBUTE_CONSTITUTION);
	if(constitution == nullptr)
		return;

	int cooldownTime = _calculateCooldownTime(walk, constitution);
	walk->startCooldown(cooldownTime);

	actor->Owner()->get<Vec2i>(PROPERTY_POSITION) += direction;
}

void ActionManager::_handleAttackAction(const std::shared_ptr<Actor> &actor, const clan::Vec2i &direction)
{
	auto pawn = actor->Owner();
	if(pawn->hasProperty(PROPERTY_POSITION) == false)
		return;

	auto position = pawn->get<Vec2i>(PROPERTY_POSITION).get();
	auto destination = position + direction;

	auto hit = actor->GetSkill(SKILL_HIT);
	if(hit == nullptr)
		return;

	auto constitution = actor->GetAttribute(ATTRIBUTE_CONSTITUTION);
	if(constitution == nullptr)
		return;

	auto strength = actor->GetAttribute(ATTRIBUTE_STRENGTH);
	if(strength == nullptr)
		return;

	int cooldownTime = _calculateCooldownTime(hit, constitution);
	hit->startCooldown(cooldownTime);

	Actor *activeTarget = nullptr;
	for(auto target : _actors)
	{
		if(target == actor.get())
			continue;

		if(target->Owner()->hasProperty(PROPERTY_POSITION) == false)
			continue;

		auto targetPosition = target->Owner()->get<Vec2i>(PROPERTY_POSITION).get();
		if(destination == targetPosition)
		{
			activeTarget = target;
			break;
		}
	}

	if(activeTarget == nullptr)
		return;

	auto targetHealth = activeTarget->GetVital(VITAL_HEALTH);
	int dmg = _calculateAttackDamage(hit, strength);
	targetHealth->setCurrentValue(targetHealth->CurrentValue() - dmg);

	std::cout << pawn->getName() << " hits " << activeTarget->Owner()->getName() << " for " << dmg << " damage!" << std::endl;
	std::cout << activeTarget->Owner()->getName() << " has " << targetHealth->CurrentValue() << " health left!" << std::endl;
}

int ActionManager::_calculateElapsedTime(const std::shared_ptr<Skill> &skill, const std::shared_ptr<Attribute> &dexterity)
{
	return max((int)floor((skill->Cost() * (dexterity->Value() * 0.1f)) + 0.5f), 0);
}

int ActionManager::_calculateCooldownTime(const std::shared_ptr<Skill> &skill, const std::shared_ptr<Attribute> &constitution)
{
	return max((int)floor((skill->CooldownCost() * (constitution->Value() * 0.1f)) + 0.5f), 0);
}

int ActionManager::_calculateAttackDamage(const std::shared_ptr<Skill> &skill, const std::shared_ptr<Attribute> &strength)
{
	return max((int)floor((skill->Value() * (strength->Value() * 0.1f)) + 0.5f), 0);
}

std::shared_ptr<Actor> ActionManager::_find(const std::shared_ptr<Entity> &owner)
{
	if(owner->hasComponent<Actor>() == false)
		return nullptr;

	return owner->getComponent<Actor>();
}
