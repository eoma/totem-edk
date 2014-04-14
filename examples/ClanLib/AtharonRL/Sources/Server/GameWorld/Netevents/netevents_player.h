#pragma once

class Player;

class NetEventsPlayer
{
// Construction:
public:
	NetEventsPlayer();

// Operations:
public:
	bool dispatch_net_event(const clan::NetGameEvent &event, Player *player);

// Events:
private:
	void on_net_event_player_move(const clan::NetGameEvent &e, Player *player);

// Implementation:
private:
	clan::NetGameEventDispatcher_v1<Player *> netevents;
};
