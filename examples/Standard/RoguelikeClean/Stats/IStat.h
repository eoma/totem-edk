#pragma once

#include <string>

class IStat
{
public:
	IStat(const std::string &name, const std::string &description) 
		: _name(name), _description(description) 
	{
		_baseValue = 0;
		_buffValue = 0;
		_experienceToLevelup = 100;
		_levelModifier = 1.1f;
	}

	virtual ~IStat() {}

	const std::string &Name() const { return _name; }
	const std::string &Description() const { return _description; }
	int BaseValue() const { return _baseValue; }
	int BuffValue() const { return _buffValue; }
	int ExperienceToLevelUp() const { return _experienceToLevelup; }
	float LevelModifier() const { return _levelModifier; }

	void setBaseValue(int value) { _baseValue = value; }
	void setBuffValue(int value) { _buffValue = value; }
	void setExperienceToLevelUp(int value) { _experienceToLevelup = value; }
	void setLevelModifier(float value) { _levelModifier = value; }

	virtual int Value() const { return _baseValue + _buffValue; }

	void LevelUp()
	{
		_experienceToLevelup = _CalculateExperienceToGainLevel();
		_baseValue++;
	}

private:
	int _CalculateExperienceToGainLevel()
	{
		return (int)floor((_experienceToLevelup * _levelModifier) + 0.5f);
	}

private:
	std::string _name;
	std::string _description;
	int _baseValue;
	int _buffValue;
	int _experienceToLevelup;
	float _levelModifier;
};
