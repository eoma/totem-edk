#pragma once

#include "GameWorld/character_manager.h"

class Player;

class NetEventsCharacter
{
// Construction:
public:
	NetEventsCharacter(clan::SqliteConnection &db, CharacterManager &character_manager);

// Operations:
public:
	bool dispatch_net_event(const clan::NetGameEvent &event, Player *player);

// Events:
private:
	void on_net_event_character_get_list(const clan::NetGameEvent &e, Player *player);
	void on_net_event_character_login(const clan::NetGameEvent &e, Player *player);
	void on_net_event_character_logout(const clan::NetGameEvent &e, Player *player);
	void on_net_event_character_create(const clan::NetGameEvent &e, Player *player);

// Implementation:
private:
	clan::NetGameEventDispatcher_v1<Player *> netevents;

	CharacterManager &character_manager;
	clan::SqliteConnection &db;
};
