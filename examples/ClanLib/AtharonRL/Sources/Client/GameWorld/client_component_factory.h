#pragma once

#include <memory>

class ClientGameObject;
class ClientZone;
class LayerManager; typedef std::shared_ptr<LayerManager> LayerManagerPtr;

class ClientComponentFactory
{
public:
	ClientComponentFactory(ClientZone *zone, const LayerManagerPtr &layer_manager);

	void create_and_add_component(ClientGameObject *owner, const std::string &type, const std::string &name);

private:
	ClientZone *zone;
	LayerManagerPtr layer_manager;
};
