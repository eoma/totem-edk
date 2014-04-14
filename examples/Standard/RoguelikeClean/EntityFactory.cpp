#include "EntityFactory.h"
#include "Entity.h"
#include "EntityContainer.h"
#include "ComponentFactory.h"

#include <libtcod.hpp>

EntityFactory::EntityFactory()
{
}

EntityFactory::~EntityFactory()
{
}

bool EntityFactory::initialize(const std::shared_ptr<EntityContainer> &entities, const Parser::StatsInfo &statsInfo, const Parser::EntitiesInfo &entitiesInfo)
{
	_entities = entities;
	_statsInfo = statsInfo;
	_entitiesInfo = entitiesInfo;

	_factory = std::unique_ptr<ComponentFactory>(new ComponentFactory());
	return true;
}

std::shared_ptr<Entity> EntityFactory::instantiate(const std::string &entityName)
{
	std::shared_ptr<Parser::EntitiesInfo::EntityInfo> info;
	for(auto entityInfo : _entitiesInfo.Entities)
	{
		if(entityInfo->Name.compare(entityName) == 0)
		{
			info = entityInfo;
			break;
		}
	}
	
	if(info == nullptr)
		return nullptr;

	auto entity = std::make_shared<Entity>(info->Name);
	
	//Add components
	for(auto componentName : info->Components)
	{
		auto component = _factory->instantiate(entity, componentName);
		
		//Some special handling required for some components...
		if(componentName.compare("Actor") == 0)
			_factory->treatActor(component, _statsInfo, *info);
	}

	//Set property data
	for(auto prop : info->IntProperties)
	{
		if(entity->hasProperty(prop->getName()))
			entity->get<int>(prop->getName()) = prop->get();
	}
	for(auto prop : info->FloatProperties)
	{
		if(entity->hasProperty(prop->getName()))
			entity->get<float>(prop->getName()) = prop->get();
	}
	for(auto prop : info->StringProperties)
	{
		if(entity->hasProperty(prop->getName()))
			entity->get<std::string>(prop->getName()) = prop->get();
	}
	for(auto prop : info->ColorProperties)
	{
		if(entity->hasProperty(prop->getName()))
			entity->get<TCODColor>(prop->getName()) = prop->get();
	}
	for(auto prop : info->Vec2iProperties)
	{
		if(entity->hasProperty(prop->getName()))
			entity->get<clan::Vec2i>(prop->getName()) = prop->get();
	}

	_entities->add(entity);
	return entity;
}
