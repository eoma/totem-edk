#pragma once

class GameObject; typedef std::shared_ptr<GameObject> GameObjectPtr;
class ClientCamera; typedef std::shared_ptr<ClientCamera> ClientCameraPtr;
class ClientVicinityObjects;
class ClientVicinityMap;
class ClientComponentFactory;
class LayerManager; typedef std::shared_ptr<LayerManager> LayerManagerPtr;

class ClientZone
{
public:
	ClientZone(clan::NetGameClient &network, const LayerManagerPtr &layer_manager);
	~ClientZone();

	void tick(float time_elapsed);

	bool dispatch_net_event(const clan::NetGameEvent &event);
	void send_event(const clan::NetGameEvent &event);

	void set_camera_target(const GameObjectPtr &camera_target) { this->camera_target = camera_target; }
	GameObjectPtr get_camera_target() const { return camera_target; }

	ClientZone &operator=(const ClientZone &rhs)
	{
		if(this == &rhs)
			return *this;

		objects = rhs.objects;
		component_factory = rhs.component_factory;
		network = rhs.network;
		camera_target = rhs.camera_target;
		return *this;
	}

	clan::Signal_v1<const ClientCameraPtr &> sig_draw;

private:
	LayerManagerPtr layer_manager;
	std::shared_ptr<ClientVicinityObjects> objects;
	std::shared_ptr<ClientVicinityMap> map;
	std::shared_ptr<ClientComponentFactory> component_factory;

	clan::NetGameClient &network;
	GameObjectPtr camera_target;
	ClientCameraPtr camera;
};