#include "precomp.h"
#include "client_vicinity_objects.h"
#include "client_gameobject.h"
#include "client_zone.h"
#include "client_component_factory.h"
#include "Engine/Common/Network/netevents.h"
#include "Engine/Common/GameWorld/property_serializer.h"

using namespace clan;

ClientVicinityObjects::ClientVicinityObjects(ClientZone *zone, std::shared_ptr<ClientComponentFactory> component_factory)
: player_gameobject_id(0), zone(zone), component_factory(component_factory)
{
	netevents.func_event(STC_OBJECT_CREATE).set(this, &ClientVicinityObjects::on_net_event_object_create);
	netevents.func_event(STC_OBJECT_UPDATE).set(this, &ClientVicinityObjects::on_net_event_object_update);
	netevents.func_event(STC_OBJECT_DESTROY).set(this, &ClientVicinityObjects::on_net_event_object_destroy);
	netevents.func_event(STC_OBJECT_PLAYER_OWN).set(this, &ClientVicinityObjects::on_net_event_object_player_own);
	netevents.func_event(STC_OBJECT_EVENT).set(this, &ClientVicinityObjects::on_net_event_object_event);
}

ClientVicinityObjects::~ClientVicinityObjects()
{
	clear();
}

bool ClientVicinityObjects::dispatch_net_event(const NetGameEvent &event)
{
	return netevents.dispatch(event);
}

void ClientVicinityObjects::add_object(const ClientGameObjectPtr &object)
{
	visible_objects.push_back(object);
}

void ClientVicinityObjects::on_net_event_object_create(const NetGameEvent &e)
{
	int a = 0;

	int id = e.get_argument(a++);
	std::string name = e.get_argument(a++);

	cl_log_event("Game", "Creating object %1 %2", id, name);

	auto gameobject = std::shared_ptr<ClientGameObject>(new ClientGameObject(zone, id, name));

	unsigned int component_count = e.get_argument(a++);
	for(size_t c = 0; c < component_count; ++c)
	{
		std::string component_type = e.get_argument(a++);
		std::string component_name = e.get_argument(a++);

		//TODO: Should fix so that entire engine uses shared_ptr instead of raw pointer!!!
		component_factory->create_and_add_component(gameobject.get(), component_type, component_name);
		cl_log_event("Game", "- Component %1 %2", component_type, component_name);

		unsigned int property_count = e.get_argument(a++);
		for(size_t p = 0; p < property_count; ++p)
		{
			int property_type = e.get_argument(a++);
			std::string property_name = e.get_argument(a++);
			std::string property_value = e.get_argument(a++);

			PropertySerializer::create_and_add_property(gameobject.get(), property_type, property_name, property_value);
			cl_log_event("Game", "  - Property %1 %2", property_name, property_value);
		}
	}

	add_object(gameobject);

	if(player_gameobject_id == id)
		zone->set_camera_target(gameobject);
}

void ClientVicinityObjects::on_net_event_object_update(const NetGameEvent &e)
{
	int a = 0;

	int id = e.get_argument(a++);

	cl_log_event("Game", "Updating object %1", id);

	auto gameobject = find_gameobject(id);
	if(gameobject)
	{
		unsigned int property_count = e.get_argument(a++);
		for(size_t p = 0; p < property_count; ++p)
		{
			int property_type = e.get_argument(a++);
			std::string property_name = e.get_argument(a++);
			std::string property_value = e.get_argument(a++);

			PropertySerializer::create_and_add_property(gameobject.get(), property_type, property_name, property_value);
			cl_log_event("Game", "- Property %1 %2", property_name, property_value);
		}
	}
}

void ClientVicinityObjects::on_net_event_object_destroy(const NetGameEvent &e)
{
	int id = e.get_argument(0);
	auto gameobject = find_gameobject(id);
	if(gameobject)
	{
		//Erases the gameobject from the visible_objects list in the update function
		gameobject->destroy();
	}
}

void ClientVicinityObjects::on_net_event_object_player_own(const NetGameEvent &e)
{
	player_gameobject_id = e.get_argument(0);

	//The flow of events is such that this should return nullptr for now...
	//but might as well check to be 110% sure :P
	//The most important thing here is to define the player_gameobject_id
	//then we set the camera target when we start creating gameobjects.
	auto gameobject = find_gameobject(player_gameobject_id);
	if(gameobject)
		zone->set_camera_target(gameobject);
}

void ClientVicinityObjects::on_net_event_object_event(const NetGameEvent &e)
{
	int gameobject_id = e.get_argument(0);

	bool handled_event = false;

	auto gameobject = find_gameobject(gameobject_id);
	if(gameobject)
	{
		std::string event_name = e.get_argument(1);
		NetGameEvent gameobject_event(event_name);
		for (size_t i = 2; i < e.get_argument_count(); i++)
			gameobject_event.add_argument(e.get_argument(i));

		handled_event = gameobject->dispatch_net_event(gameobject_event);
	}

	if (!handled_event)
		cl_log_event("Network", "Unhandled gameobject event: %1", e.to_string());
}

ClientGameObjectPtr ClientVicinityObjects::find_gameobject(int id)
{
	for (auto it = visible_objects.begin(); it != visible_objects.end(); ++it)
	{
		auto gameobject = *it;
		if (gameobject->get_id() == id)
			return gameobject;
	}
	return 0;
}

void ClientVicinityObjects::clear()
{
	visible_objects.clear(); //Shared ptr only needs to clear list
}

void ClientVicinityObjects::update(float time_elapsed)
{
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		visible_objects[i]->update(time_elapsed);
	}
	
	for (size_t i = visible_objects.size(); i > 0; i--)
	{
		if (visible_objects[i-1]->is_destroyed())
			visible_objects.erase(visible_objects.begin() + (i-1)); //Shared ptr only needs to erase from list
	}
}
