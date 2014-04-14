#pragma once

#include "client_map_tile.h"
class ClientZone;

class ClientVicinityMap
{
// Construction:
public:
	ClientVicinityMap(ClientZone *zone, const LayerManagerPtr &layer_manager);
	~ClientVicinityMap();

// Attributes:
public:

// Operations:
public:
	bool dispatch_net_event(const clan::NetGameEvent &event);
	
	void update(float time_elapsed);
	void draw(const ClientCameraPtr &camera);
	
	void clear();

// Implementation:
private:
	void on_net_event_map_create(const clan::NetGameEvent &e);
	void on_net_event_map_destroy(const clan::NetGameEvent &e);
	void on_net_event_map_event(const clan::NetGameEvent &e);
	void on_net_event_map_update(const clan::NetGameEvent &e);

	std::vector<ClientMapTilePtr> visible_tiles;

	clan::NetGameEventDispatcher_v0 netevents;

	int player_gameobject_id;

	ClientZone *zone;
	LayerManagerPtr layer_manager;
};
