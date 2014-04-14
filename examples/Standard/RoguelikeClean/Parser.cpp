#include "Parser.h"
#include "PropertyDefinitions.h"
#include "Components/ComponentDefinitions.h"

Parser::Parser(const std::string &resourceDir)
	: resourceDir(resourceDir)
{
	parser = new TCODParser();
}

Parser::~Parser()
{
	delete parser;
}

Parser::CfgInfo Parser::parseCfg(const std::string &file)
{
	auto dataStruct = parser->newStructure("config");

	dataStruct
		->addProperty("width", TCOD_TYPE_INT, false)
		->addProperty("height", TCOD_TYPE_INT, false)
		->addProperty("fullscreen", TCOD_TYPE_BOOL, false)
		->addProperty("font", TCOD_TYPE_STRING, false)
		->addProperty("title", TCOD_TYPE_STRING, false)
		->addProperty("introMessage", TCOD_TYPE_STRING, false);

	Parser::CfgInfo info;
	auto tmp = resourceDir;
	parser->run(tmp.append(file).c_str(), new CfgParserListener(info));
	return info;
}

Parser::StatsInfo Parser::parseStats(const std::string &file)
{
	auto attribStruct = parser->newStructure("attribute");
	attribStruct
		->addProperty("description", TCOD_TYPE_STRING, true)
		->addProperty("experienceToLevelUp", TCOD_TYPE_INT, false)
		->addProperty("levelModifier", TCOD_TYPE_FLOAT, false)
		->addProperty("baseValue", TCOD_TYPE_INT, false);

	auto vitalStruct = parser->newStructure("vital");
	vitalStruct
		->addProperty("description", TCOD_TYPE_STRING, true)
		->addProperty("experienceToLevelUp", TCOD_TYPE_INT, false)
		->addProperty("levelModifier", TCOD_TYPE_FLOAT, false)
		->addProperty("baseValue", TCOD_TYPE_INT, false)
		->addListProperty("modifierNames", TCOD_TYPE_STRING, false)
		->addListProperty("modifierValues", TCOD_TYPE_FLOAT, false);

	auto skillStruct = parser->newStructure("skill");
	skillStruct
		->addProperty("description", TCOD_TYPE_STRING, true)
		->addProperty("experienceToLevelUp", TCOD_TYPE_INT, false)
		->addProperty("levelModifier", TCOD_TYPE_FLOAT, false)
		->addProperty("baseValue", TCOD_TYPE_INT, false)
		->addProperty("experienceAwardedForUse", TCOD_TYPE_INT, false)
		->addProperty("cost", TCOD_TYPE_INT, false)
		->addProperty("cooldown", TCOD_TYPE_INT, false)
		->addProperty("activated", TCOD_TYPE_BOOL, false)
		->addListProperty("modifierNames", TCOD_TYPE_STRING, false)
		->addListProperty("modifierValues", TCOD_TYPE_FLOAT, false);

	Parser::StatsInfo info;
	auto tmp = resourceDir;
	parser->run(tmp.append(file).c_str(), new StatsParserListener(info));
	return info;
}

Parser::EntitiesInfo Parser::parseEntities(const std::string &file)
{
	auto tagStruct = parser->newStructure("tags");
	tagStruct
		->addFlag("Abstract")
		->addFlag("Playable")
		->addFlag("Item");

	auto compStruct = parser->newStructure("components");
	compStruct
		->addFlag(COMPONENT_ACTOR)
		->addFlag(COMPONENT_VISUAL)
		->addFlag(COMPONENT_AI);

	auto entityStruct = parser->newStructure("entity");
	entityStruct
		->addProperty(PROPERTY_LEVEL, TCOD_TYPE_INT, false)
		->addProperty(PROPERTY_EXPERIENCE_TO_SPEND, TCOD_TYPE_INT, false)
		->addProperty(PROPERTY_CANVAS_LAYER, TCOD_TYPE_INT, false)
		->addProperty(PROPERTY_SIGHT_RADIUS, TCOD_TYPE_INT, false)
		->addProperty(PROPERTY_SYMBOL, TCOD_TYPE_CHAR, false)
		->addProperty(PROPERTY_FOREGROUND_COLOR, TCOD_TYPE_COLOR, false)
		->addListProperty(PROPERTY_POSITION, TCOD_TYPE_INT, false)
		->addListProperty("attributes", TCOD_TYPE_INT, false)
		->addListProperty("vitals", TCOD_TYPE_INT, false)
		->addListProperty("skills", TCOD_TYPE_INT, false)
		->addStructure(tagStruct)
		->addStructure(compStruct);

	Parser::EntitiesInfo info;
	auto tmp = resourceDir;
	parser->run(tmp.append(file).c_str(), new EntitiesParserListener(info));
	return info;
}

Parser::MapsInfo Parser::parseMaps(const std::string &file)
{
	auto dataStruct = parser->newStructure("map");

	dataStruct
		->addProperty("layer", TCOD_TYPE_INT, true)
		->addProperty("width", TCOD_TYPE_INT, true)
		->addProperty("height", TCOD_TYPE_INT, true)
		->addProperty("generationType", TCOD_TYPE_INT, true)
		->addProperty("wallInViewColor", TCOD_TYPE_COLOR, true)
		->addProperty("groundInViewColor", TCOD_TYPE_COLOR, true)
		->addProperty("wallInMemoryColor", TCOD_TYPE_COLOR, true)
		->addProperty("groundInMemoryColor", TCOD_TYPE_COLOR, true);

	Parser::MapsInfo info;
	auto tmp = resourceDir;
	parser->run(tmp.append(file).c_str(), new MapsParserListener(info));
	return info;
}

///////////////////////////////////////////////////
//
// CONFIG PARSER LISTENER
//
///////////////////////////////////////////////////
CfgParserListener::CfgParserListener(Parser::CfgInfo &info)
	: ITCODParserListener(), info(&info)
{
}

CfgParserListener::~CfgParserListener()
{
}

bool CfgParserListener::parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name)
{
	std::string struct_name = str->getName();
	if(struct_name.compare("config") == 0)
	{
		
	}
    return true;
}
bool CfgParserListener::parserFlag(TCODParser *parser,const char *name)
{
	return true;
}
bool CfgParserListener::parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value)
{
	std::string propName = name;

	if(propName.compare("width") == 0 && type == TCOD_TYPE_INT)
	{
		if(info != nullptr) info->Width = value.i;
	}
	else if(propName.compare("height") == 0 && type == TCOD_TYPE_INT)
	{
		if(info != nullptr) info->Height = value.i;
	}
	else if(propName.compare("fullscreen") == 0 && type == TCOD_TYPE_BOOL)
	{
		if(info != nullptr) info->Fullscreen = value.b;
	}
	else if(propName.compare("font") == 0 && type == TCOD_TYPE_STRING)
	{
		if(info != nullptr) info->Font = value.s;
	}
	else if(propName.compare("title") == 0 && type == TCOD_TYPE_STRING)
	{
		if(info != nullptr) info->Title = value.s;
	}
	else if(propName.compare("introMessage") == 0 && type == TCOD_TYPE_STRING)
	{
		if(info != nullptr) info->IntroMessage = value.s;
	}
	else 
	{
		return false;
	}

	return true;
}
bool CfgParserListener::parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name)
{
	return true;
}
void CfgParserListener::error(const char *msg)
{
	fprintf(stderr,msg);
}

///////////////////////////////////////////////////
//
// STATS PARSER LISTENER
//
///////////////////////////////////////////////////
StatsParserListener::StatsParserListener(Parser::StatsInfo &info)
	: ITCODParserListener(), info(&info), currentStructIsAttrib(false), currentStructIsVital(false), currentStructIsSkill(false)
{
}

StatsParserListener::~StatsParserListener()
{
}

bool StatsParserListener::parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name)
{
	std::string struct_name = str->getName();
	if(struct_name.compare("attribute") == 0)
	{
		currentStructIsAttrib = true;
		currentStructIsVital = false;
		currentStructIsSkill = false;
		auto attribInfo = std::make_shared<Parser::StatsInfo::AttributeInfo>();
		attribInfo->Name = name;
		info->Attributes.push_back(attribInfo);
	}
	else if(struct_name.compare("vital") == 0)
	{
		currentStructIsAttrib = false;
		currentStructIsVital = true;
		currentStructIsSkill = false;
		auto vitalInfo = std::make_shared<Parser::StatsInfo::VitalInfo>();
		vitalInfo->Name = name;
		info->Vitals.push_back(vitalInfo);
	}
	else if(struct_name.compare("skill") == 0)
	{
		currentStructIsAttrib = false;
		currentStructIsVital = false;
		currentStructIsSkill = true;
		auto skillInfo = std::make_shared<Parser::StatsInfo::SkillInfo>();
		skillInfo->Name = name;
		info->Skills.push_back(skillInfo);
	}
	else
		return false;

    return true;
}
bool StatsParserListener::parserFlag(TCODParser *parser,const char *name)
{
	return true;
}
bool StatsParserListener::parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value)
{
	std::string propName = name;
	if(currentStructIsAttrib)
	{
		auto attribInfo = info->Attributes.back();
		if(propName.compare("description") == 0 && type == TCOD_TYPE_STRING)
		{
			attribInfo->Description = value.s;
		}
		else if(propName.compare("experienceToLevelUp") == 0 && type == TCOD_TYPE_INT)
		{
			attribInfo->ExperienceToLevelUp = value.i;
		}
		else if(propName.compare("levelModifier") == 0 && type == TCOD_TYPE_FLOAT)
		{
			attribInfo->LevelModifier = value.f;
		}
		else if(propName.compare("baseValue") == 0 && type == TCOD_TYPE_INT)
		{
			attribInfo->BaseValue = value.i;
		}
		else 
		{
			return false;
		}
	}
	else if(currentStructIsVital)
	{
		auto vitalInfo = info->Vitals.back();
		if(propName.compare("description") == 0 && type == TCOD_TYPE_STRING)
		{
			vitalInfo->Description = value.s;
		}
		else if(propName.compare("experienceToLevelUp") == 0 && type == TCOD_TYPE_INT)
		{
			vitalInfo->ExperienceToLevelUp = value.i;
		}
		else if(propName.compare("levelModifier") == 0 && type == TCOD_TYPE_FLOAT)
		{
			vitalInfo->LevelModifier = value.f;
		}
		else if(propName.compare("baseValue") == 0 && type == TCOD_TYPE_INT)
		{
			vitalInfo->BaseValue = value.i;
		}
		else if(propName.compare("modifierNames") == 0 && type == TCOD_TYPE_LIST | TCOD_TYPE_STRING)
		{
			auto list = TCODList<char*>(value.list);
			for(auto it = list.begin(); it != list.end(); ++it)
			{
				vitalInfo->ModifierNames.push_back(*it);
			}
		}
		else if(propName.compare("modifierValues") == 0 && type == TCOD_TYPE_LIST | TCOD_TYPE_FLOAT)
		{
			auto list = TCODList<float>(value.list);
			for(auto it = list.begin(); it != list.end(); ++it)
			{
				vitalInfo->ModifierValues.push_back(*it);
			}
		}
		else 
		{
			return false;
		}
	}
	else if(currentStructIsSkill)
	{
		auto skillInfo = info->Skills.back();
		if(propName.compare("description") == 0 && type == TCOD_TYPE_STRING)
		{
			skillInfo->Description = value.s;
		}
		else if(propName.compare("experienceToLevelUp") == 0 && type == TCOD_TYPE_INT)
		{
			skillInfo->ExperienceToLevelUp = value.i;
		}
		else if(propName.compare("levelModifier") == 0 && type == TCOD_TYPE_FLOAT)
		{
			skillInfo->LevelModifier = value.f;
		}
		else if(propName.compare("baseValue") == 0 && type == TCOD_TYPE_INT)
		{
			skillInfo->BaseValue = value.i;
		}
		else if(propName.compare("experienceAwardedForUse") == 0 && type == TCOD_TYPE_INT)
		{
			skillInfo->ExperienceAwardedForUse = value.i;
		}
		else if(propName.compare("cost") == 0 && type == TCOD_TYPE_INT)
		{
			skillInfo->Cost = value.i;
		}
		else if(propName.compare("cooldown") == 0 && type == TCOD_TYPE_INT)
		{
			skillInfo->Cooldown = value.i;
		}
		else if(propName.compare("activated") == 0 && type == TCOD_TYPE_BOOL)
		{
			skillInfo->Activated = value.b;
		}
		else if(propName.compare("modifierNames") == 0 && type == TCOD_TYPE_LIST | TCOD_TYPE_STRING)
		{
			auto list = TCODList<char*>(value.list);
			for(auto it = list.begin(); it != list.end(); ++it)
			{
				skillInfo->ModifierNames.push_back(*it);
			}
		}
		else if(propName.compare("modifierValues") == 0 && type == TCOD_TYPE_LIST | TCOD_TYPE_FLOAT)
		{
			auto list = TCODList<float>(value.list);
			for(auto it = list.begin(); it != list.end(); ++it)
			{
				skillInfo->ModifierValues.push_back(*it);
			}
		}
		else 
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}
bool StatsParserListener::parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name)
{
	return true;
}
void StatsParserListener::error(const char *msg)
{
	fprintf(stderr,msg);
}

///////////////////////////////////////////////////
//
// ENTITIES PARSER LISTENER
//
///////////////////////////////////////////////////
EntitiesParserListener::EntitiesParserListener(Parser::EntitiesInfo &info)
	: ITCODParserListener(), info(&info)
{
}

EntitiesParserListener::~EntitiesParserListener()
{
}

bool EntitiesParserListener::parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name)
{
	std::string struct_name = str->getName();
	if(struct_name.compare("entity") == 0)
	{
		currentStructIsBase = true;
		currentStructIsTags = false;
		currentStructIsComps = false;

		auto entityInfo = std::make_shared<Parser::EntitiesInfo::EntityInfo>();
		entityInfo->Name = name;
		info->Entities.push_back(entityInfo);
	}
	else if(struct_name.compare("tags") == 0)
	{
		currentStructIsBase = false;
		currentStructIsTags = true;
		currentStructIsComps = false;
	}
	else if(struct_name.compare("components") == 0)
	{
		currentStructIsBase = false;
		currentStructIsTags = false;
		currentStructIsComps = true;
	}
    return true;
}
bool EntitiesParserListener::parserFlag(TCODParser *parser,const char *name)
{
	auto entityInfo = info->Entities.back();
	if(currentStructIsTags)
		entityInfo->Tags.push_back(name);
	else if(currentStructIsComps)
		entityInfo->Components.push_back(name);
	else
		return false;

	return true;
}
bool EntitiesParserListener::parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value)
{
	if(currentStructIsBase == false)
		return false;

	std::string propName = name;
	auto entityInfo = info->Entities.back();

	if(propName.compare("attributes") == 0 && type == TCOD_TYPE_LIST | TCOD_TYPE_INT)
	{
		auto list = TCODList<int>(value.list);
		for(auto it = list.begin(); it != list.end(); ++it)
		{
			entityInfo->AttributeValues.push_back(*it);
		}
	}
	else if(propName.compare("vitals") == 0 && type == TCOD_TYPE_LIST | TCOD_TYPE_INT)
	{
		auto list = TCODList<int>(value.list);
		for(auto it = list.begin(); it != list.end(); ++it)
		{
			entityInfo->VitalValues.push_back(*it);
		}
	}
	else if(propName.compare("skills") == 0 && type == TCOD_TYPE_LIST | TCOD_TYPE_INT)
	{
		auto list = TCODList<int>(value.list);
		for(auto it = list.begin(); it != list.end(); ++it)
		{
			entityInfo->SkillValues.push_back(*it);
		}
	}
	else 
	{
		if(type == TCOD_TYPE_INT)
		{
			auto prop = std::make_shared<Totem::Property<int>>(propName);
			prop->set(value.i, false);
			entityInfo->IntProperties.push_back(prop);
		}
		else if(type == TCOD_TYPE_CHAR)
		{
			auto prop = std::make_shared<Totem::Property<int>>(propName);
			prop->set((int)value.c, false);
			entityInfo->IntProperties.push_back(prop);
		}
		else if(type == TCOD_TYPE_FLOAT)
		{
			auto prop = std::make_shared<Totem::Property<float>>(propName);
			prop->set(value.f, false);
			entityInfo->FloatProperties.push_back(prop);
		}
		else if(type == TCOD_TYPE_STRING)
		{
			auto prop = std::make_shared<Totem::Property<std::string>>(propName);
			prop->set(value.s, false);
			entityInfo->StringProperties.push_back(prop);
		}
		else if(type == TCOD_TYPE_COLOR)
		{
			auto prop = std::make_shared<Totem::Property<TCODColor>>(propName);
			prop->set(value.col, false);
			entityInfo->ColorProperties.push_back(prop);
		}
		else if(type == TCOD_TYPE_LIST | TCOD_TYPE_INT)
		{
			auto list = TCODList<int>(value.list);
			if(list.size() == 2)
			{
				clan::Vec2i v(list.get(0), list.get(1));
				auto prop = std::make_shared<Totem::Property<clan::Vec2i>>(propName);
				prop->set(v, false);
				entityInfo->Vec2iProperties.push_back(prop);
			}
		}
		else
			return false;
	}

	return true;
}
bool EntitiesParserListener::parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name)
{
	std::string struct_name = str->getName();
	if(struct_name.compare("tags") == 0)
	{
		currentStructIsTags = false;
		currentStructIsBase = true;
	}
	else if(struct_name.compare("components") == 0)
	{
		currentStructIsComps = false;
		currentStructIsBase = true;
	}
	return true;
}
void EntitiesParserListener::error(const char *msg)
{
	fprintf(stderr,msg);
}

///////////////////////////////////////////////////
//
// MAPS PARSER LISTENER
//
///////////////////////////////////////////////////
MapsParserListener::MapsParserListener(Parser::MapsInfo &info)
	: ITCODParserListener(), info(&info)
{
}

MapsParserListener::~MapsParserListener()
{
}

bool MapsParserListener::parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name)
{
	std::string struct_name = str->getName();
	if(struct_name.compare("map") == 0)
	{
		auto mapInfo = std::make_shared<Parser::MapsInfo::MapInfo>();
		mapInfo->Name = name;
		info->Maps.push_back(mapInfo);
	}
    return true;
}
bool MapsParserListener::parserFlag(TCODParser *parser,const char *name)
{
	return true;
}
bool MapsParserListener::parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value)
{
	std::string propName = name;
	auto mapInfo = info->Maps.back();

	if(propName.compare("layer") == 0 && type == TCOD_TYPE_INT)
	{
		mapInfo->Layer = value.i;
	}
	else if(propName.compare("width") == 0 && type == TCOD_TYPE_INT)
	{
		mapInfo->Width = value.i;
	}
	else if(propName.compare("height") == 0 && type == TCOD_TYPE_INT)
	{
		mapInfo->Height = value.i;
	}
	else if(propName.compare("generationType") == 0 && type == TCOD_TYPE_INT)
	{
		mapInfo->GenerationType = value.i;
	}
	else if(propName.compare("wallInViewColor") == 0 && type == TCOD_TYPE_COLOR)
	{
		mapInfo->WallInViewColor = value.col;
	}
	else if(propName.compare("groundInViewColor") == 0 && type == TCOD_TYPE_COLOR)
	{
		mapInfo->GroundInViewColor = value.col;
	}
	else if(propName.compare("wallInMemoryColor") == 0 && type == TCOD_TYPE_COLOR)
	{
		mapInfo->WallInMemoryColor = value.col;
	}
	else if(propName.compare("groundInMemoryColor") == 0 && type == TCOD_TYPE_COLOR)
	{
		mapInfo->GroundInMemoryColor = value.col;
	}
	else 
	{
		return false;
	}

	return true;
}
bool MapsParserListener::parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name)
{
	return true;
}
void MapsParserListener::error(const char *msg)
{
	fprintf(stderr,msg);
}