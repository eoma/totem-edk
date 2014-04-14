#include "precomp.h"
#include "player.h"
#include "character.h"

using namespace clan;

Player::Player(SqliteConnection &db, int user_id, NetGameConnection *connection)
: db(db), connection(connection), user_id(user_id), character(0)
{
}

Player::~Player()
{
	delete character;
}

void Player::send_event(const NetGameEvent &event)
{
	connection->send_event(event);
}

Character *Player::create_character(int character_id, const std::string &character_name, ServerGameObject *gameobject)
{
	character = new Character(db, character_id, character_name, this, gameobject);

	return character;
}
