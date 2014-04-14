#pragma once

class ServerGameObject;

class ComponentFactory
{
public:
	ComponentFactory();

	void create_and_add_component(ServerGameObject *owner, const std::string &type, const std::string &name);
};
typedef std::shared_ptr<ComponentFactory> ComponentFactoryPtr;
