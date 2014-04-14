
#pragma once

class Player;

//TODO: Implement this as a proper Totem component

class NetworkReceiver
{
public:
	virtual bool dispatch_net_event(const clan::NetGameEvent &event, Player *player) = 0;
};
