#include "precomp.h"
#include "gameobject_container.h"
#include "server_gameobject.h"
#include "Database/database_gameobject_containers.h"
#include "Database/database_gameobjects.h"
#include "Engine/Common/GameWorld/property_serializer.h"
#include <algorithm>

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

GameObjectContainer::GameObjectContainer(SqliteConnection &db, int container_id)
: container_id(container_id), db(db)
{
}

GameObjectContainer::~GameObjectContainer()
{
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
		delete *it;
	gameobjects.clear();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

ServerGameObject *GameObjectContainer::find(const std::string &name) const
{
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		ServerGameObject *object = *it;
		if(object->get_name() == name)
			return object;
	}
	return 0;
}

ServerGameObject *GameObjectContainer::find(int id) const
{
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		ServerGameObject *object = *it;
		if(object->get_id() == id)
			return object;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void GameObjectContainer::load_from_database(std::shared_ptr<ComponentFactory> component_factory)
{
	cl_log_event("Zones", "Loading gameobjects in container %1", container_id);

	std::vector<int> gameobject_ids = DatabaseGameObjectContainers::get_load_by_default_gameobject_ids_in_container(db, container_id);
	for(size_t i = 0; i < gameobject_ids.size(); ++i)
	{
		ServerGameObject *gameobject = load_gameobject_from_database(gameobject_ids[i], component_factory);
		gameobjects.push_back(gameobject);
	}

	cl_log_event("Zones", "Loaded %1 gameobjects", gameobjects.size());
}

ServerGameObject *GameObjectContainer::load_gameobject_from_database(int gameobject_id, std::shared_ptr<ComponentFactory> component_factory)
{
	DatabaseGameObjects::GameObjectInfo info = DatabaseGameObjects::get_info(db, gameobject_id);
	//if(info.load_by_default == false)
	//	throw Exception("GameObject should not be loaded at server startup (load_by_default must be true)");

	std::vector<DatabaseGameObjects::GameObjectComponentInfo> components = DatabaseGameObjects::get_components(db, gameobject_id);
	std::vector<DatabaseGameObjects::GameObjectPropertyInfo> properties = DatabaseGameObjects::get_properties(db, gameobject_id);

	cl_log_event("Debug", "Creating gameobject %1", info.description);
	ServerGameObject *gameobject = new ServerGameObject(gameobject_id, info.description, db);

	for(size_t i = 0; i < components.size(); ++i)
	{
		component_factory.get()->create_and_add_component(gameobject, components[i].type, components[i].name);
		cl_log_event("Debug", "- Added component %1 %2", components[i].type, components[i].name);
	}
	for(size_t i = 0; i < properties.size(); ++i)
	{
		PropertySerializer::create_and_add_property(gameobject, properties[i].type_id, properties[i].name, properties[i].value);
//		cl_log_event("Debug", "- Added property %1 %2", properties[i].name, properties[i].value);
	}

	gameobject->set_persist_changes(true);

	return gameobject;
}

void GameObjectContainer::update(float time_elapsed)
{
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		ServerGameObject *gameobject = (*it);	
		gameobject->update(time_elapsed);
	}
}

void GameObjectContainer::save_dirty_properties()
{
	for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
	{
		ServerGameObject *gameobject = (*it);	
		gameobject->save_dirty_properties();
	}
}

bool GameObjectContainer::add(ServerGameObject *gameobject)
{
	if(gameobject)
	{
		auto it = std::find(gameobjects.begin(), gameobjects.end(), gameobject);
		if (it == gameobjects.end())
		{
			gameobjects.push_back(gameobject);
			gameobject->set_container_id(container_id);
			gameobject->save_dirty_properties();

			return true;
		}
	}

	return false;
}

bool GameObjectContainer::remove(ServerGameObject *gameobject)
{
	auto it = std::find(gameobjects.begin(), gameobjects.end(), gameobject);
	if (it != gameobjects.end())
	{
		gameobjects.erase(it);
		gameobject->set_container_id(0);
		return true;
	}
	return false;
}

bool GameObjectContainer::set_inactive(ServerGameObject *gameobject)
{
	auto it = std::find(gameobjects.begin(), gameobjects.end(), gameobject);
	if (it != gameobjects.end())
	{
		gameobjects.erase(it);
		//FIXME: Should probably not change a default load here, since that's about how objects are loaded at
		//server startup, and not which objects in the database are active right now...
		//DatabaseGameObjects::set_gameobject_load_state(db, gameobject->get_id(), false);
		return true;
	}
	return false;
}
