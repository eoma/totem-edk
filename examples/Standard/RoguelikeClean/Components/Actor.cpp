#include "Actor.h"
#include "../GameManager.h"
#include "../ActionManager.h"
#include "../PropertyDefinitions.h"
#include "../EventDefinitions.h"

using namespace clan;
using namespace Totem;

Actor::Actor(Entity *owner)
	: Component<Actor>(GetType()), _owner(owner)
{
	_level = owner->add<int>(PROPERTY_LEVEL, 0);
	_experienceToSpend = owner->add<int>(PROPERTY_EXPERIENCE_TO_SPEND, 0);
	_sightRadius = owner->add<int>(PROPERTY_SIGHT_RADIUS, 10);

	owner->registerToEvent1<int>(EVENT_NewTurn).connect(this, &Actor::OnNewTurn);

	GameManager::Get()->getAction()->add(this);
}

Actor::~Actor() 
{
	GameManager::Get()->getAction()->remove(this);
}

bool Actor::initialize(const Parser::StatsInfo &statsInfo, const Parser::EntitiesInfo::EntityInfo &entityInfo)
{
	for(auto info : statsInfo.Attributes)
	{
		auto attribute = std::make_shared<Attribute>(info->Name, info->Description, info->ExperienceToLevelUp, info->LevelModifier);
		attribute->setBaseValue(info->BaseValue);
		_attributes.push_back(attribute);
	}

	for(auto info : statsInfo.Vitals)
	{
		auto vital = std::make_shared<Vital>(info->Name, info->Description, info->ExperienceToLevelUp, info->LevelModifier);
		vital->setBaseValue(info->BaseValue);
		vital->setCurrentValue(vital->Value());
		
		for(unsigned int i = 0; i < info->ModifierNames.size(); i++)
		{
			auto attribute = GetAttribute(info->ModifierNames[i]);
			if(attribute && i < info->ModifierValues.size())
				vital->Add(std::make_shared<AttributeModifier>(attribute, info->ModifierValues[i]));
		}

		_vitals.push_back(vital);
	}

	for(auto info : statsInfo.Skills)
	{
		auto skill = std::shared_ptr<Skill>(new Skill(info->Name, info->Description, info->ExperienceToLevelUp, info->LevelModifier, info->Cost, info->Cooldown));
		skill->setBaseValue(info->BaseValue);
		skill->setActive(info->Activated);

		for(unsigned int i = 0; i < info->ModifierNames.size(); i++)
		{
			auto attribute = GetAttribute(info->ModifierNames[i]);
			if(attribute && i < info->ModifierValues.size())
				skill->Add(std::make_shared<AttributeModifier>(attribute, info->ModifierValues[i]));
		}

		_skills.push_back(skill);
	}

	for(unsigned int i = 0; i < entityInfo.AttributeValues.size(); i++)
	{
		if(i < _attributes.size())
		{
			auto attribute = _attributes[i];
			attribute->setBuffValue(attribute->BuffValue() + entityInfo.AttributeValues[i]);
		}
		else
		{
			return false;
		}
	}

	for(unsigned int i = 0; i < entityInfo.VitalValues.size(); i++)
	{
		if(i < _vitals.size())
		{
			auto vital = _vitals[i];
			vital->setBuffValue(vital->BuffValue() + entityInfo.VitalValues[i]);
			vital->setCurrentValue(vital->Value());
		}
		else
		{
			return false;
		}
	}

	for(unsigned int i = 0; i < entityInfo.SkillValues.size(); i++)
	{
		if(i < _skills.size())
		{
			auto skill = _skills[i];
			skill->setBuffValue(skill->BuffValue() + entityInfo.SkillValues[i]);
		}
		else
		{
			return false;
		}
	}

	_UpdateStats(0);
	return true;
}

std::shared_ptr<Attribute> Actor::GetAttribute(const std::string &name) const
{
	for(auto value : _attributes)
	{
		if(value->Name() == name)
			return value;
	}

	return nullptr;
}

std::shared_ptr<Vital> Actor::GetVital(const std::string &name) const
{
	for(auto value : _vitals)
	{
		if(value->Name() == name)
			return value;
	}

	return nullptr;
}

std::shared_ptr<Skill> Actor::GetSkill(const std::string &name) const
{
	for(auto value : _skills)
	{
		if(value->Name() == name)
			return value;
	}

	return nullptr;
}

void Actor::AddExperience(int experience)
{
	_experienceToSpend += experience;
	_CalculateLevel();
}

//Take average of all of the actor's skills and assign that as the actor level
void Actor::_CalculateLevel()
{
}

void Actor::_UpdateStats(int elapsedTime)
{
	for(auto vital : _vitals)
		vital->Update(elapsedTime);
	for(auto skill : _skills)
		skill->Update(elapsedTime);
}

void Actor::OnNewTurn(int elapsedTime)
{
	_UpdateStats(elapsedTime);
}
