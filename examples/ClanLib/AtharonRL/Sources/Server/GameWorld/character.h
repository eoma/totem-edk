#pragma once

class Zone;
class Player;
class ServerGameObject;

class Character
{
// Construction:
public:
	Character(clan::SqliteConnection &db, int character_id, const std::string &character_name, Player *player, ServerGameObject *gameobject);
	~Character();

// Attributes:
public:
	ServerGameObject *get_owner_gameobject() const { return owner_gameobject; }

	int get_id() const { return character_id; }
	std::string get_name() const { return character_name; }

// Operations:
public:
	void move_to_zone(Zone *new_zone, bool set_start_position);
	void remove_from_zone();

// Implementation:
private:
	void set_owner_gameobject(ServerGameObject *gameobject);

	Zone *zone;
	Player *player;

	int character_id;
	std::string character_name;

	ServerGameObject *owner_gameobject;

	clan::SqliteConnection &db;
};
