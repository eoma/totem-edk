#pragma once

#include "IStat.h"

class Attribute : public IStat
{
public:
	Attribute(const std::string &name, const std::string &description, int ExperienceToLevelUp, float levelModifier) : IStat(name, description)
	{
		setExperienceToLevelUp(ExperienceToLevelUp);
		setLevelModifier(levelModifier);
	}
};
