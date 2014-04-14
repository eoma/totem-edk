#include "precomp.h"
#include "component_factory.h"
#include "server_gameobject.h"
#include "Components/server_ascii_visual.h"
#include "Components/server_transform.h"

using namespace Totem;
using namespace clan;

ComponentFactory::ComponentFactory()
{
}

void ComponentFactory::create_and_add_component(ServerGameObject *owner, const std::string &type, const std::string &name)
{
	if(type == ServerAsciiVisual::getType())
	{
		owner->addComponent(std::make_shared<ServerAsciiVisual>(owner, name));
	}
	else if(type == ServerTransform::getType())
	{
		owner->addComponent(std::make_shared<ServerTransform>(owner, name));
	}
	else
	{
		throw Exception("ComponentFactory::create_and_add_component - Unknown component type " + type);
	}
}
