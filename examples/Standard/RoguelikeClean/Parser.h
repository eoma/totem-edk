#pragma once

#include "Math/vec2.h"

#include <Totem/Property.h>
#include <libtcod.hpp>
#include <memory>
#include <vector>

class Parser
{
public:
	Parser(const std::string &resourceDir);
	~Parser();

	struct CfgInfo
	{
		int Width;
		int Height;
		bool Fullscreen;
		std::string Font;
		std::string Title;
		std::string IntroMessage;
	};
	CfgInfo parseCfg(const std::string &file);

	struct StatsInfo
	{
		struct AttributeInfo
		{
			std::string Name;
			std::string Description;
			int ExperienceToLevelUp;
			float LevelModifier;
			int BaseValue;
		};

		struct VitalInfo
		{
			std::string Name;
			std::string Description;
			int ExperienceToLevelUp;
			float LevelModifier;
			int BaseValue;
			std::vector<std::string> ModifierNames;
			std::vector<float> ModifierValues;
		};

		struct SkillInfo
		{
			std::string Name;
			std::string Description;
			int ExperienceToLevelUp;
			float LevelModifier;
			int BaseValue;
			std::vector<std::string> ModifierNames;
			std::vector<float> ModifierValues;
			int ExperienceAwardedForUse;
			int Cost;
			int Cooldown;
			bool Activated;
		};

		std::vector<std::shared_ptr<AttributeInfo>> Attributes;
		std::vector<std::shared_ptr<VitalInfo>> Vitals;
		std::vector<std::shared_ptr<SkillInfo>> Skills;
	};
	StatsInfo parseStats(const std::string &file);

	struct EntitiesInfo
	{
		struct EntityInfo
		{
			std::string Name;
			std::vector<std::string> Tags;
			std::vector<std::string> Components;
			std::vector<int> AttributeValues;
			std::vector<int> VitalValues;
			std::vector<int> SkillValues;

			std::vector<std::shared_ptr<Totem::Property<int>>> IntProperties;
			std::vector<std::shared_ptr<Totem::Property<float>>> FloatProperties;
			std::vector<std::shared_ptr<Totem::Property<std::string>>> StringProperties;
			std::vector<std::shared_ptr<Totem::Property<TCODColor>>> ColorProperties;
			std::vector<std::shared_ptr<Totem::Property<clan::Vec2i>>> Vec2iProperties;
		};
		std::vector<std::shared_ptr<EntityInfo>> Entities;
	};
	EntitiesInfo parseEntities(const std::string &file);

	struct MapsInfo
	{
		struct MapInfo
		{
			std::string Name;
			int Layer;
			int Width;
			int Height;
			int GenerationType;
			TCODColor WallInViewColor;
			TCODColor GroundInViewColor;
			TCODColor WallInMemoryColor;
			TCODColor GroundInMemoryColor;
		};
		std::vector<std::shared_ptr<MapInfo>> Maps;
	};
	MapsInfo parseMaps(const std::string &file);

private:
	TCODParser *parser;
	std::string resourceDir;
};

class CfgParserListener : public ITCODParserListener
{
public:
	CfgParserListener(Parser::CfgInfo &info);
	~CfgParserListener();

private:
	bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name) override;
	bool parserFlag(TCODParser *parser,const char *name) override;
	bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value) override;
	bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name) override;
	void error(const char *msg) override;

	Parser::CfgInfo *info;
};

class StatsParserListener : public ITCODParserListener
{
public:
	StatsParserListener(Parser::StatsInfo &info);
	~StatsParserListener();

private:
	bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name) override;
	bool parserFlag(TCODParser *parser,const char *name) override;
	bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value) override;
	bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name) override;
	void error(const char *msg) override;

	Parser::StatsInfo *info;
	bool currentStructIsAttrib;
	bool currentStructIsVital;
	bool currentStructIsSkill;
};

class EntitiesParserListener : public ITCODParserListener
{
public:
	EntitiesParserListener(Parser::EntitiesInfo &info);
	~EntitiesParserListener();

private:
	bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name) override;
	bool parserFlag(TCODParser *parser,const char *name) override;
	bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value) override;
	bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name) override;
	void error(const char *msg) override;

	Parser::EntitiesInfo *info;
	bool currentStructIsBase;
	bool currentStructIsTags;
	bool currentStructIsComps;
};

class MapsParserListener : public ITCODParserListener
{
public:
	MapsParserListener(Parser::MapsInfo &info);
	~MapsParserListener();

private:
	bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name) override;
	bool parserFlag(TCODParser *parser,const char *name) override;
	bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value) override;
	bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name) override;
	void error(const char *msg) override;

	Parser::MapsInfo *info;
};
