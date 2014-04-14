#include "precomp.h"
#include "server_gameobject.h"
#include "Database/database_gameobjects.h"
#include "Database/database_gameobject_containers.h"
#include "Engine/Common/Network/netevents.h"
#include "Engine/Common/GameWorld/property_serializer.h"
#include "Engine/Common/GameWorld/component_serializer.h"
#include <algorithm>

using namespace Totem;
using namespace clan;

ServerGameObject::ServerGameObject(int id, const std::string &name, SqliteConnection &db)
: GameObject(id, name), db(db), persist_changes(false)
{
	slots.connect(propertyAdded(), this, &ServerGameObject::on_property_added);
	slots.connect(propertyRemoved(), this, &ServerGameObject::on_property_removed);
	slots.connect(componentAdded(), this, &ServerGameObject::on_component_added);
	slots.connect(componentRemoved(), this, &ServerGameObject::on_component_removed);
}

void ServerGameObject::set_container_id(int container_id)
{ 
	this->container_id = container_id; 

	DatabaseGameObjectContainers::move_gameobject_to_container(db, id, container_id);
}

void ServerGameObject::set_persist_changes(bool enable)
{
	persist_changes = enable;
}

void ServerGameObject::on_property_added(std::shared_ptr<IProperty> property)
{
	if(persist_changes)
	{
		DatabaseGameObjects::add_property(db, id, PropertySerializer::get_property_type(property), property->getName(), PropertySerializer::property_value_to_string(property));
		property->clearDirty();
	}
}

void ServerGameObject::on_property_removed(std::shared_ptr<IProperty> property)
{
	if(persist_changes)
		DatabaseGameObjects::remove_property(db, id, property->getName());
}

void ServerGameObject::on_component_added(std::shared_ptr<IComponent<>> component)
{
	if(persist_changes)
		DatabaseGameObjects::add_component(db, id, ComponentSerializer::get_component_type(component), component->getName());
}

void ServerGameObject::on_component_removed(std::shared_ptr<IComponent<>> component)
{
	if(persist_changes)
		DatabaseGameObjects::remove_component(db, id, ComponentSerializer::get_component_type(component), component->getName());
}

void ServerGameObject::save_dirty_properties() 
{
	std::vector<DatabaseGameObjects::GameObjectDirtyPropertyInfo> dirty_properties;

	std::vector<std::shared_ptr<IComponent<>>> &components = getComponents();
	std::vector<std::shared_ptr<IComponent<>>>::iterator cit;
	for(cit = components.begin(); cit != components.end(); ++cit)
	{
		std::shared_ptr<IComponent<>> component = (*cit);
		std::unordered_map<std::string, std::shared_ptr<IProperty>> &properties = component->getProperties();
		for(auto it = properties.begin(); it != properties.end(); ++it)
		{
			std::shared_ptr<IProperty> property = (*it).second;
			if(property->isDirty())
			{
				DatabaseGameObjects::GameObjectDirtyPropertyInfo database_property;
				database_property.gameobject_id = id;
				database_property.component_name = component->getName();
				database_property.property_name = property->getName();
				database_property.property_value = PropertySerializer::property_value_to_string(property);
				dirty_properties.push_back(database_property);
			}
		}
	}

	std::unordered_map<std::string, std::shared_ptr<IProperty>> &properties = getProperties();
	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		std::shared_ptr<IProperty> property = (*it).second;
		if(property->isDirty())
		{
			DatabaseGameObjects::GameObjectDirtyPropertyInfo database_property;
			database_property.gameobject_id = id;
			database_property.property_name = property->getName();
			database_property.property_value = PropertySerializer::property_value_to_string(property);
			dirty_properties.push_back(database_property);
		}
	}

	if(!dirty_properties.empty()) 
	{
		DatabaseGameObjects::save_dirty_properties(db, dirty_properties);
		clearDirtyProperties();
	}
}

void ServerGameObject::send_event(const NetGameEvent &event, NetGameConnection *player_connection)
{
	NetGameEvent marshalled_event(STC_OBJECT_EVENT, id, event.get_name());
	for (size_t i = 0; i < event.get_argument_count(); i++)
		marshalled_event.add_argument(event.get_argument(i));

	player_connection->send_event(marshalled_event);
}
