#pragma once

#include "Engine/Common/GameWorld/map_tile_interface.h"

class LayerManager; typedef std::shared_ptr<LayerManager> LayerManagerPtr;
class Layer; typedef std::shared_ptr<Layer> LayerPtr;
class ClientZone;
class ClientCamera; typedef std::shared_ptr<ClientCamera> ClientCameraPtr;

class ClientMapTile; typedef std::shared_ptr<ClientMapTile> ClientMapTilePtr;

class ClientMapTile : public MapTileInterface
{
public:
	ClientMapTile(const clan::Vec2i &position, bool walkable, ClientZone *zone, const LayerManagerPtr &layer_manager);
	virtual ~ClientMapTile();

	void update(float time_elapsed);
	void draw(const ClientCameraPtr &camera);

protected:
	LayerManagerPtr layer_manager;
	LayerPtr dungeon_layer;
	bool walkable;
};
