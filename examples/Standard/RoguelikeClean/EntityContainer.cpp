#include "EntityContainer.h"
#include "Entity.h"
#include "EventDefinitions.h"

EntityContainer::EntityContainer()
{
}

EntityContainer::~EntityContainer()
{
}

std::shared_ptr<Entity> EntityContainer::find(const std::string &name) const
{
	for(auto entity : _entities)
	{
		if(entity->getName() == name)
			return entity;
	}

	return nullptr;
}

void EntityContainer::update()
{
	for(auto entity : _pendingDelete)
	{
		remove(entity, true);
	}
	_pendingDelete.clear();

	for(auto entity : _entities)
	{
		entity->updateComponents(1);
		entity->updateProperties();
		entity->updatePropertyLists();
	}
}

bool EntityContainer::add(const std::shared_ptr<Entity> &entity)
{
	if(entity)
	{
		auto it = std::find(_entities.begin(), _entities.end(), entity);
		if(it == _entities.end())
		{
			_entities.push_back(entity);
			return true;
		}
	}
	return false;
}

bool EntityContainer::remove(const std::shared_ptr<Entity> &entity, bool immediate)
{
	if(entity)
	{
		auto it = std::find(_entities.begin(), _entities.end(), entity);
		if(it != _entities.end())
		{
			if(immediate)
				_entities.erase(it);
			else
				_pendingDelete.push_back(entity);
			return true;
		}
	}
	return false;
}

void EntityContainer::think(int elapsedTime)
{
	//for(auto entity : _entities)
	//	entity->sendEvent1<int>(EVENT_MoveOrInteract, elapsedTime);
}
