#pragma once

class Player;

class PlayerManager
{
// Construction:
public:
	PlayerManager(clan::SqliteConnection &db);
	~PlayerManager();

// Attributes:
public:
	Player *find_player(int user_id) const;
	Player *find_player(clan::NetGameConnection *connection) const;
	Player *find_player_with_character(int character_id) const;

	const std::vector<Player *> &get_players() const { return players; }

// Operations:
public:
	void create_player(int user_id, clan::NetGameConnection *connection);
	void remove_player(clan::NetGameConnection *connection);
	void remove_player(Player *player);
	void kick_player(Player *player, const std::string &reason);

// Implementation:
private:
	std::vector<Player *> players;

	clan::SqliteConnection &db;
};
