#include "precomp.h"
#include "client_gameobject.h"
#include "client_zone.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

ClientGameObject::ClientGameObject(ClientZone *zone, int id, const std::string &name)
: GameObject(id, name), zone(zone)
{
}

bool ClientGameObject::dispatch_net_event(const NetGameEvent &event)
{
	return false;
}

void ClientGameObject::send_event(const NetGameEvent &event)
{
	NetGameEvent marshalled_event(CTS_OBJECT_EVENT, id, event.get_name());
	for (size_t i = 0; i < event.get_argument_count(); i++)
		marshalled_event.add_argument(event.get_argument(i));

	zone->send_event(marshalled_event);
}
