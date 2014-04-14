#include "EntityParser.h"
#include "Engine.h"
#include "Entity.h"

#include "Systems/PrefabSystem.h"
#include "Systems/MonsterSystem.h"

#include "Components/Actor.h"
#include "Components/Map.h"
#include "Components/Destructible.h"
#include "Components/Attacker.h"
#include "Components/Player.h"
#include "Components/Monster.h"
#include "Components/Gui.h"
#include "Components/Container.h"
#include "Components/Consumable.h"
#include "Components/Pickable.h"
#include "Components/Magic/Effect.h"
#include "Components/Magic/Healer.h"
#include "Components/Magic/Weave.h"
#include "Utils/TargetSelector.h"

EntityParser::EntityParser()
{
	parser = new TCODParser();
	auto entity_struct = parser->newStructure("entity");

	//Available components
	entity_struct
		->addFlag("Actor")
		->addFlag("Ai")
		->addFlag("Attacker")
		->addFlag("Consumable")
		->addFlag("Container")
		->addFlag("Destructible")
		->addFlag("Monster")
		->addFlag("Pickable")
		->addFlag("Player")
		//Magic
		->addFlag("Effect")
		->addFlag("Healer")
		->addFlag("Weave");

	//Available properties
	entity_struct
		->addProperty("Character", TCOD_TYPE_CHAR, false)
		->addProperty("Color", TCOD_TYPE_COLOR, false)
		->addProperty("Defense", TCOD_TYPE_FLOAT, false)
		->addProperty("MaxHP", TCOD_TYPE_FLOAT, false)
		->addProperty("HP", TCOD_TYPE_FLOAT, false)
		->addProperty("Power", TCOD_TYPE_FLOAT, false)
		->addProperty("InventoryMaxSize", TCOD_TYPE_INT, false)
		->addProperty("CorpseName", TCOD_TYPE_STRING, false)
		->addProperty("Level", TCOD_TYPE_INT, false)
		->addProperty("Family", TCOD_TYPE_STRING, false)

		->addProperty("Blocks", TCOD_TYPE_BOOL, false)
		->addProperty("Amount", TCOD_TYPE_FLOAT, false)
		->addProperty("HealerMessage", TCOD_TYPE_STRING, false)
		->addProperty("TargetSelector", TCOD_TYPE_STRING, false)
		->addProperty("TargetRange", TCOD_TYPE_FLOAT, false);

	entity_struct->addStructure(entity_struct);

	parser->run((Engine::getSingleton()->getResourceDir() + "Entities/Player.cfg").c_str(), new EntityParserListener());
	parser->run((Engine::getSingleton()->getResourceDir() + "Entities/Monsters.cfg").c_str(), new EntityParserListener());
	parser->run((Engine::getSingleton()->getResourceDir() + "Entities/Items.cfg").c_str(), new EntityParserListener());
}

EntityParser::~EntityParser()
{
	delete parser;
}

///////////////////////////////////////////////////
//
// ENTITY PARSER LISTENER
//
///////////////////////////////////////////////////
EntityParserListener::EntityParserListener()
	: ITCODParserListener(), info(new EntityInfo())
{
}

EntityParserListener::~EntityParserListener()
{
	delete info;
}

bool EntityParserListener::parserNewStruct(TCODParser *parser,const TCODParserStruct *str,const char *name)
{
	printf ("new structure type '%s' with name '%s'\n", str->getName(), name ? name : "NULL");
	std::string struct_name = str->getName();
	if(struct_name == "entity")
	{
		//If this is a root structure, and the name of the structure is empty,
		//that means this is a new root structure
		if(info->parent == nullptr && info->name.empty())
		{
			info->name = name;
		}
		//Else, if the root structure has already been defined, but there's no parent definition yet,
		//that means that this is the first child structure of the root structure.
		else if(info->parent == nullptr)
		{
			auto child_info = EntityInfo();
			child_info.parent = info;
			info->children.push_back(child_info);

			//Swap info pointer to current child
			info = &info->children[info->children.size()-1];
			info->name = name;
		}
		//Else we already have one or more child structures defined, thus we're adding
		//another child to the parent of the current info pointer...
		else
		{
			info = info->parent;
			auto child_info = EntityInfo();
			child_info.parent = info;
			info->children.push_back(child_info);

			//Swap info pointer to current child
			info = &info->children[info->children.size()-1];
			info->name = name;
		}
	}
    return true;
}
bool EntityParserListener::parserFlag(TCODParser *parser,const char *name)
{
	printf ("found new flag '%s'\n",name);
	if(info->name.empty())
		return false;

	//Special handling of some types
	if(std::string(name) == "Monster")
		info->monsterSystemEntry = true;

	info->components.push_back(name);
	return true;
}
bool EntityParserListener::parserProperty(TCODParser *parser,const char *name, TCOD_value_type_t type, TCOD_value_t value)
{
	printf ("found new property '%s'\n",name);
	if(info->name.empty() || (info->parent == nullptr && info->components.empty()) || (info->parent != nullptr && info->parent->components.empty() && info->components.empty()))
		return false;

	std::string prop_name = name;

	auto engine = Engine::getSingleton();

	//Check for special types of properties first...
	if(prop_name == "TargetSelector")
	{
		auto property = new Totem::Property<std::string>(prop_name);
		property->set(value.s, false);
		info->special_properties.push_back(property);
	}
	else if(prop_name == "TargetRange")
	{
		auto property = new Totem::Property<float>(prop_name);
		property->set(value.f, false);
		info->special_properties.push_back(property);
	}
	else if(prop_name == "Family")
	{
		auto property = new Totem::Property<int>(prop_name);
		std::string family = value.s;
		if(family == "Goblin" || family == "GOBLIN" || family == "goblin")
		{
			property->set((int)GOBLIN_FAMILY, false);
			info->monsterInfo.family = GOBLIN_FAMILY;
		}
		else if(family == "Human" || family == "HUMAN" || family == "human")
		{
			property->set((int)HUMAN_FAMILY, false);
			info->monsterInfo.family = HUMAN_FAMILY;
		}
		else
		{
			delete property;
			return false;
		}
		info->special_properties.push_back(property);
	}
	else if(prop_name == "Level")
	{
		info->monsterInfo.level = value.i;
	}

	//Then we handle for generic types...
	if(type == TCOD_TYPE_BOOL)
	{
		auto property = new Totem::Property<bool>(prop_name);
		property->set((bool)value.b, false);
		info->properties.push_back(property);
	}
	else if(type == TCOD_TYPE_CHAR)
	{
		auto property = new Totem::Property<int>(prop_name);
		property->set((int)value.c, false);
		info->properties.push_back(property);
	}
	else if(type == TCOD_TYPE_INT)
	{
		auto property = new Totem::Property<int>(prop_name);
		property->set(value.i, false);
		info->properties.push_back(property);
	}
	else if(type == TCOD_TYPE_FLOAT)
	{
		auto property = new Totem::Property<float>(prop_name);
		property->set(value.f, false);
		info->properties.push_back(property);
	}
	else if(type == TCOD_TYPE_STRING)
	{
		auto property = new Totem::Property<std::string>(prop_name);
		property->set(value.s, false);
		info->properties.push_back(property);
	}
	else if(type == TCOD_TYPE_COLOR)
	{
		auto property = new Totem::Property<TCODColor>(prop_name);
		property->set(value.col, false);
		info->properties.push_back(property);
	}
	return true;
}
bool EntityParserListener::parserEndStruct(TCODParser *parser,const TCODParserStruct *str, const char *name)
{
	printf ("end of structure type '%s'\n",name);

	//Make sure everything is nice and dandy
	if( info->name.empty() || info->name != std::string(name) )
		return false;

	auto prefab_system = Engine::getSingleton()->getPrefabSystem();

	//If the current entity info is the root structure, then we should just close off and clear out the info
	if(info->parent == nullptr)
	{
		//If the root structure has children, then we define it as abstract. Thus only those structures with
		//a parent can be defined as a prefab!
		if(info->children.empty())
		{
			prefab_system->createPrefab(info->name, info->components, info->properties, info->special_properties);

			//Check if this is a monster system entry
			if(info->monsterSystemEntry)
			{
				auto monster_system = Engine::getSingleton()->getMonsterSystem();
				monster_system->createFamilyPrefab(info->name, info->monsterInfo.family, info->monsterInfo.level);
			}
		}

		//Reset the entity info, in case there are more root structures defined in the current file we're parsing.
		info->name.clear();
		info->components.clear();
		info->properties.clear();
		info->special_properties.clear();
		info->children.clear();
		info->monsterSystemEntry = false;
	}
	//Else, this is a child structure of the root, thus we need to make sure we get all the components of our parent
	//structure as well as our own.
	else
	{
		//We extract the data into new structures, so that the parent info comes first in the list. This is important if
		//we are to support children overriding the data of their parent. This is specially important for the properties!
		std::vector<std::string> components;
		std::vector<Totem::IProperty*> properties;
		std::vector<Totem::IProperty*> special_properties;
		
		for(unsigned int i = 0; i < info->parent->components.size(); i++)
			components.push_back(info->parent->components[i]);
		for(unsigned int i = 0; i < info->parent->properties.size(); i++)
			properties.push_back(info->parent->properties[i]);
		for(unsigned int i = 0; i < info->parent->special_properties.size(); i++)
			special_properties.push_back(info->parent->special_properties[i]);

		for(unsigned int i = 0; i < info->components.size(); i++)
			components.push_back(info->components[i]);
		for(unsigned int i = 0; i < info->properties.size(); i++)
			properties.push_back(info->properties[i]);
		for(unsigned int i = 0; i < info->special_properties.size(); i++)
			special_properties.push_back(info->special_properties[i]);

		prefab_system->createPrefab(info->name, components, properties, special_properties);

		//Check if this is a monster system entry
		if(info->monsterSystemEntry || info->parent->monsterSystemEntry)
		{
			auto monster_system = Engine::getSingleton()->getMonsterSystem();
			monster_system->createFamilyPrefab(info->name, info->monsterInfo.family, info->monsterInfo.level);
		}

		//Reset the current entity info and define the root structure as current info again, so that if there's
		//any new components/properties defined after this on the root, then it will be added to the correct
		//info structure (even though it wouldn't make much sense to do so). 
		//Also, it just flows with the architecture better to do this...
		info->name.clear();
		info->components.clear();
		info->properties.clear();
		info->special_properties.clear();
		info->monsterSystemEntry = false;
		info = info->parent;
	}

	return true;
}
void EntityParserListener::error(const char *msg)
{
	fprintf(stderr,msg);
}
