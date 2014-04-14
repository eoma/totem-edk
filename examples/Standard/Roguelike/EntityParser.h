#pragma once

//Need to handle monsters as special entities...
#include "Components/Monster.h"

#include <Totem/Property.h>
#include <libtcod.hpp>
#include <vector>

class Entity;
typedef std::shared_ptr<Entity> EntityPtr;

class EntityParser
{
public:
	EntityParser();
	~EntityParser();

private:
	TCODParser *parser;
};

class EntityParserListener : public ITCODParserListener
{
public:
	EntityParserListener();
	~EntityParserListener();

	bool parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name) override;
	bool parserFlag(TCODParser *parser,const char *name) override;
	bool parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value) override;
	bool parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name) override;
	void error(const char *msg) override;

private:
	struct MonsterInfo
	{
		MonsterFamily family;
		int level;

		MonsterInfo() : family(GOBLIN_FAMILY), level(0) {}
	};
	struct EntityInfo
	{
		std::string name;
		std::vector<std::string> components;
		std::vector<Totem::IProperty*> properties;
		std::vector<Totem::IProperty*> special_properties;
		std::vector<EntityInfo> children;
		EntityInfo *parent;
		bool monsterSystemEntry;
		MonsterInfo monsterInfo;

		EntityInfo() : parent(nullptr), monsterSystemEntry(false) {}
	};
	EntityInfo *info;
};
