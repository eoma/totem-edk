#pragma once

class Character;
class ServerGameObject;

class Player
{
// Construction:
public:
	Player(clan::SqliteConnection &db, int user_id, clan::NetGameConnection *connection);
	~Player();

// Attributes:
public:
	clan::NetGameConnection *get_connection() const { return connection; }

	int get_user_id() const { return user_id; }

	Character *get_character() const { return character; }

// Operations:
public:
	Character *create_character(int character_id, const std::string &character_name, ServerGameObject *gameobject);

	void send_event(const clan::NetGameEvent &event);

// Implementation:
private:
	clan::NetGameConnection *connection;
	clan::SqliteConnection &db;

	int user_id;

	Character *character;
};
