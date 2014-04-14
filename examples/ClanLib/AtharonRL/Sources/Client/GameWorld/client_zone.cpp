#include "precomp.h"
#include "client_zone.h"
#include "client_vicinity_objects.h"
#include "client_vicinity_map.h"
#include "client_component_factory.h"
#include "client_camera.h"
#include "client_gameobject.h"
#include "Engine/Client/Scene/layer_manager.h"
#include "Engine/Client/Scene/layer.h"

using namespace clan;

ClientZone::ClientZone(clan::NetGameClient &network, const LayerManagerPtr &layer_manager)
: network(network), layer_manager(layer_manager)
{
	component_factory = std::make_shared<ClientComponentFactory>(this, layer_manager);
	objects = std::make_shared<ClientVicinityObjects>(this, component_factory);
	map = std::make_shared<ClientVicinityMap>(this, layer_manager);

	camera = std::make_shared<ClientCamera>(layer_manager->get_screen_size()/2); //project to center of layer buffer
}

ClientZone::~ClientZone()
{
}

void ClientZone::tick(float time_elapsed)
{
	objects->update(time_elapsed);
	map->update(time_elapsed);

	if(camera_target && camera_target->hasProperty("Position"))
		camera->set_view(camera_target->get<Vec2i>("Position").get());

	map->draw(camera);
	sig_draw.invoke(camera);
}

bool ClientZone::dispatch_net_event(const NetGameEvent &event)
{
//	cl_log_event("Game", "Event: %1", event.to_string());
	bool handled = objects->dispatch_net_event(event);

	if( !handled )
		handled = map->dispatch_net_event(event);


	return handled;
}

void ClientZone::send_event(const NetGameEvent &event)
{
	network.send_event(event);
}
