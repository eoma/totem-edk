#pragma once

#include "Engine/Common/GameWorld/gameobject.h"

class ClientZone;

class ClientGameObject : public GameObject
{
public:
	ClientGameObject(ClientZone *zone, int id, const std::string &name);

	void send_event(const clan::NetGameEvent &event);

	bool dispatch_net_event(const clan::NetGameEvent &event);

private:
	ClientZone *zone;
};

typedef std::shared_ptr<GameObject> GameObjectPtr;
typedef std::shared_ptr<ClientGameObject> ClientGameObjectPtr;
