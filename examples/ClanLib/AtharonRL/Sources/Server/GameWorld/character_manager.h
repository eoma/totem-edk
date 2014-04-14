#pragma once

#include "Zone/zone_manager.h"

class Player;

class CharacterManager
{
// Construction:
public:
	CharacterManager(clan::SqliteConnection &db, ZoneManager &zone_manager);

// Operations:
public:
	void login_character(int character_id, Player *player);

	int create_character(int user_id, const std::string &character_name);

// Implementation:
private:
	void initialize_character(int character_id, Player *player);

	clan::SqliteConnection &db;
	ZoneManager &zone_manager;
};
