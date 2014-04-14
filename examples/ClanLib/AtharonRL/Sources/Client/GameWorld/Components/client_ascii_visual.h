
#pragma once

#include "Engine/Common/GameWorld/Components/ascii_visual.h"

class LayerManager; typedef std::shared_ptr<LayerManager> LayerManagerPtr;
class Layer; typedef std::shared_ptr<Layer> LayerPtr;
class ClientZone;
class ClientCamera; typedef std::shared_ptr<ClientCamera> ClientCameraPtr;

class ClientAsciiVisual : public AsciiVisual
{
public:
	ClientAsciiVisual(GameObject *owner, const std::string &name, ClientZone *zone, const LayerManagerPtr &layer_manager);
	virtual ~ClientAsciiVisual();

	void update(const float &delta_time) override;

private:
	LayerManagerPtr layer_manager;
	LayerPtr character_layer;
	LayerPtr dungeon_layer;

	Totem::Property<clan::Vec2i> position_property;

	void on_draw(const ClientCameraPtr &camera);
	clan::SlotContainer slots;
};
