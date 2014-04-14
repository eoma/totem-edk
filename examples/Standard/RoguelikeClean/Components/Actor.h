#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"
#include "../Math/vec2.h"

#include "../Stats/Attribute.h"
#include "../Stats/Vital.h"
#include "../Stats/Skill.h"

#include "../Parser.h"

#include "ComponentDefinitions.h"

class Actor : public Totem::Component<Actor>
{
public:
	Actor(Entity *owner);
	virtual ~Actor();

	static std::string GetType() { return COMPONENT_ACTOR; }
	Entity *Owner() const { return _owner; }

public:
	bool initialize(const Parser::StatsInfo &statsInfo, const Parser::EntitiesInfo::EntityInfo &entityInfo);

	int Level() const { return _level; }
	int ExperienceToSpend() const { return _experienceToSpend; }

	std::shared_ptr<Attribute> GetAttribute(const std::string &name) const;
	std::shared_ptr<Vital> GetVital(const std::string &name) const;
	std::shared_ptr<Skill> GetSkill(const std::string &name) const;

	void setLevel(int value) { _level = value; }
	void setExperienceToSpend(int value) { _experienceToSpend = value; }

public:
	void AddExperience(int experience);

private:
	void _CalculateLevel();
	void _UpdateStats(int elapsedTime);

private:
	Entity *_owner;

	Totem::Property<int> _level;
	Totem::Property<int> _experienceToSpend;
	Totem::Property<int> _sightRadius;

	std::vector<std::shared_ptr<Attribute>> _attributes;
	std::vector<std::shared_ptr<Vital>> _vitals;
	std::vector<std::shared_ptr<Skill>> _skills;

	void OnNewTurn(int elapsedTime);
};
