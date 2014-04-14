#pragma once

#include "ModifiedStat.h"

class Vital : public ModifiedStat
{
public:
	Vital(const std::string &name, const std::string &description, int experienceToLevelUp, float levelModifier) 
		: ModifiedStat(name, description)
	{
		setExperienceToLevelUp(experienceToLevelUp);
		setLevelModifier(levelModifier);
		_currentValue = 0;
	}
	virtual ~Vital() {}

	int CurrentValue() const { return _currentValue; }
	void setCurrentValue(int value) { _currentValue = value; }

private:
	int _currentValue;
};
