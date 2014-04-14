#include "precomp.h"
#include "client_component_factory.h"
#include "client_gameobject.h"
#include "Components/client_ascii_visual.h"
#include "Components/client_transform.h"

using namespace Totem;
using namespace clan;

ClientComponentFactory::ClientComponentFactory(ClientZone *zone, const LayerManagerPtr &layer_manager)
: layer_manager(layer_manager), zone(zone)
{
}

void ClientComponentFactory::create_and_add_component(ClientGameObject *owner, const std::string &type, const std::string &name)
{
	if(type == ClientAsciiVisual::getType())
	{
		owner->addComponent(std::make_shared<ClientAsciiVisual>(owner, name, zone, layer_manager));
	}
	else if(type == ClientTransform::getType())
	{
		owner->addComponent(std::make_shared<ClientTransform>(owner, name));
	}
	else
	{
		throw Exception("ClientComponentFactory::create_and_add_component - Unknown component type " + type);
	}
}
