#pragma once

#include "Parser.h"
#include <Totem/IComponent.h>
#include <memory>
#include <string>

class Entity;

class ComponentFactory
{
public:
	ComponentFactory();
	~ComponentFactory();

	std::shared_ptr<Totem::IComponent<>> instantiate(const std::shared_ptr<Entity> &owner, const std::string &componentName);

	//Support special handling for some component types, if someone request it
	bool treatActor(const std::shared_ptr<Totem::IComponent<>> &component, const Parser::StatsInfo &statsInfo, const Parser::EntitiesInfo::EntityInfo &entityInfo);
};