#pragma once

#include "GameWorld/player_manager.h"
#include "GameWorld/character_manager.h"
#include "GameWorld/Zone/zone_manager.h"
#include "GameWorld/Netevents/netevents_character.h"
#include "GameWorld/Netevents/netevents_player.h"

class ServerUser;

class Server
{
public:
	Server();
	~Server();

	void run(clan::Event &stop_event);

private:
	void initialize_network();

	void on_client_connected(clan::NetGameConnection *connection);
	void on_client_disconnected(clan::NetGameConnection *connection);

	void on_net_event_received(clan::NetGameConnection *connection, const clan::NetGameEvent &e);
	void on_net_event_login(const clan::NetGameEvent &e, ServerUser *user);
	void on_net_event_ping(const clan::NetGameEvent &e, ServerUser *user);

	clan::SlotContainer slots;

	clan::NetGameServer network;
	clan::NetGameEventDispatcher_v1<ServerUser *> netevents;

	ZoneManager zone_manager;
	PlayerManager player_manager;
	CharacterManager character_manager;
	NetEventsCharacter netevents_character;
	NetEventsPlayer netevents_player;

	clan::SqliteConnection db;
};
