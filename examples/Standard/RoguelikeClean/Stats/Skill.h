#pragma once

#include "ModifiedStat.h"

class Skill : public ModifiedStat
{
public:
	Skill(const std::string &name, const std::string &description, int experienceToLevelUp, float levelModifier, int actionCost, int cooldownCost) 
		: ModifiedStat(name, description)
	{
		setExperienceToLevelUp(experienceToLevelUp);
		setLevelModifier(levelModifier);
		_active = false;
		_actionCost = actionCost;
		_cooldownCost = cooldownCost;
		_currentCooldownTime = 0;
	}
	virtual ~Skill() {}

	bool Active() const { return _active; }
	int ActionCost() const { return _actionCost; }
	int CooldownCost() const { return _cooldownCost; }
	
	int Cost() const { return (int)floor((ActionCost() * Value()) * 0.1f + 0.5f); }

	void setActive(bool value) { _active = value; }
	void setActionCost(int value) { _actionCost = value; }
	void setCooldownCost(int value) { _cooldownCost = value; }

	void startCooldown(int time) { _currentCooldownTime = time; }
	bool isOnCooldown() const { return _currentCooldownTime > 0; }

	void Update(int elapsedTime) override
	{
		ModifiedStat::Update(elapsedTime);

		if(_currentCooldownTime > 0)
			_currentCooldownTime -= elapsedTime;
	}

private:
	bool _active;
	int _actionCost;
	int _cooldownCost;
	int _currentCooldownTime;
};
