#pragma once

#include "Parser.h"

class Entity;
class EntityContainer;
class ComponentFactory;

class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();

	bool initialize(const std::shared_ptr<EntityContainer> &entities, const Parser::StatsInfo &statsInfo, const Parser::EntitiesInfo &entitiesInfo);

	std::shared_ptr<Entity> instantiate(const std::string &entityName);

private:
	std::unique_ptr<ComponentFactory> _factory;

	std::shared_ptr<EntityContainer> _entities;
	Parser::StatsInfo _statsInfo;
	Parser::EntitiesInfo _entitiesInfo;
};