#include "precomp.h"
#include "zone_vicinity_objects.h"
#include "zone.h"
#include "GameWorld/server_gameobject.h"
#include "GameWorld/replicated_component.h"
#include "Engine/Common/Network/netevents.h"
#include "Engine/Common/GameWorld/component_serializer.h"
#include "Engine/Common/GameWorld/property_serializer.h"
#include <algorithm>

using namespace Totem;
using namespace clan;

ZoneVicinityObjects::ZoneVicinityObjects(NetGameConnection *connection)
: connection(connection) 
{
}

ZoneVicinityObjects::~ZoneVicinityObjects()
{
	visible_objects.clear();
}

ServerGameObject *ZoneVicinityObjects::find(int id) const
{
	std::vector<ServerGameObject *>::const_iterator it;
	for (it = visible_objects.begin(); it != visible_objects.end(); ++it)
	{
		ServerGameObject *object = *it;
		if(object->get_id() == id)
			return object;
	}

	return 0;
}

const std::vector<ServerGameObject *> &ZoneVicinityObjects::get_visible_objects() const
{
	return visible_objects;
}

void ZoneVicinityObjects::add_visible_object(ServerGameObject *gameobject)
{
	// TODO; What about shared properties?
	// TODO; support component properties

	visible_objects.push_back(gameobject);

	NetGameEvent create_event(STC_OBJECT_CREATE);
	create_event.add_argument(gameobject->get_id());
	create_event.add_argument(gameobject->get_name());

	auto &components = gameobject->getComponents();

	unsigned int replicateComponentCount = 0;
	for(auto it = components.begin(); it != components.end(); ++it)
	{
		if(dynamic_cast<ReplicatedComponent *> ((*it).get()))
		{
			replicateComponentCount++;
		}
	}
	create_event.add_argument(replicateComponentCount);

	for(auto it = components.begin(); it != components.end(); ++it)
	{
		std::shared_ptr<IComponent<>> comp = (*it);
		ReplicatedComponent *replicated_component = dynamic_cast<ReplicatedComponent *> (comp.get());
		if(replicated_component)
		{
			create_event.add_argument(ComponentSerializer::get_component_type(comp));
			create_event.add_argument(comp->getName());

			std::vector<IProperty *> dirty_properties = replicated_component->get_tracked_properties();
			unsigned int dirty_properties_count = dirty_properties.size();
			create_event.add_argument(dirty_properties_count);
			for(size_t i = 0; i < dirty_properties_count; ++i)
			{
				create_event.add_argument(PropertySerializer::get_property_type(dirty_properties[i]));
				create_event.add_argument(dirty_properties[i]->getName());
				create_event.add_argument(PropertySerializer::property_value_to_string(dirty_properties[i]));
			}
			cl_log_event("Debug", "Replicated component %1 on gameobject %2", ComponentSerializer::get_component_type(comp), gameobject->get_name());
		}
	}

	connection->send_event(create_event);
}

void ZoneVicinityObjects::clear_dirty_gameobjects()
{
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		ServerGameObject *gameobject = visible_objects[i];

		auto &components = gameobject->getComponents();
		for(auto it = components.begin(); it != components.end(); ++it)
		{
			std::shared_ptr<IComponent<>> comp = (*it);
			ReplicatedComponent *replicated_component = dynamic_cast<ReplicatedComponent *> (comp.get());
			if(replicated_component)
			{
				replicated_component->clear_dirty_properties();
			}
		}
	}
}

void ZoneVicinityObjects::sync_dirty_gameobjects()
{
	// TODO; support component properties

	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		ServerGameObject *gameobject = visible_objects[i];

		NetGameEvent create_event(STC_OBJECT_UPDATE);
		create_event.add_argument(gameobject->get_id());
		bool has_dirty_properties = false;

		auto &components = gameobject->getComponents();
		for(auto it = components.begin(); it != components.end(); ++it)
		{
			std::shared_ptr<IComponent<>> comp = (*it);
			ReplicatedComponent *replicated_component = dynamic_cast<ReplicatedComponent *> (comp.get());
			if(replicated_component)
			{
				std::vector<IProperty *> dirty_properties = replicated_component->get_dirty_properties();
				if(dirty_properties.size() > 0)
				{
					has_dirty_properties = true;

					unsigned int dirty_properties_count = dirty_properties.size();
					create_event.add_argument(dirty_properties_count);
					for(size_t i = 0; i < dirty_properties_count; ++i)
					{
						create_event.add_argument(PropertySerializer::get_property_type(dirty_properties[i]));
						create_event.add_argument(dirty_properties[i]->getName());
						create_event.add_argument(PropertySerializer::property_value_to_string(dirty_properties[i]));
					}

					cl_log_event("Debug", "Replicated updated component %1 on gameobject %2", ComponentSerializer::get_component_type(comp), gameobject->get_name());
				}
			}
		}

		if(has_dirty_properties) {
			connection->send_event(create_event);
		}
	}
}

void ZoneVicinityObjects::remove_visible_object(ServerGameObject *object)
{
	std::vector<ServerGameObject *>::iterator it = std::find(visible_objects.begin(), visible_objects.end(), object);
	if (it == visible_objects.end())
		throw Exception("Object not in visible_objects collection!");
	visible_objects.erase(it);

	NetGameEvent destroyEvent(STC_OBJECT_DESTROY, object->get_id());
	connection->send_event(destroyEvent);
}

void ZoneVicinityObjects::gameobject_position_changed(ServerGameObject *gameobject)
{
	if (is_object_in_visibility(gameobject))
	{
		// Todo: remove it from visibility if it became invisible.

/*		NetGameEvent changeEvent(
			"PositionUpdate", 
			gameobject->GetId(), 
			gameobject->GetPosition().x, 
			gameobject->GetPosition().y);
		connection->send_event(changeEvent);
*/	}
	else
	{
		// Todo: only add it to visibility if it became visible.
	}
}

void ZoneVicinityObjects::add_gameobject(ServerGameObject *gameobject)
{
	// Todo: only add it to visibility if it is actually visible.
	add_visible_object(gameobject);
}

void ZoneVicinityObjects::remove_gameobject(ServerGameObject *gameobject)
{
	remove_visible_object(gameobject);
}

bool ZoneVicinityObjects::is_object_in_visibility(ServerGameObject *gameobject) const
{
	return std::find(visible_objects.begin(), visible_objects.end(), gameobject) != visible_objects.end();
}
