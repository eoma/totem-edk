#include "precomp.h"
#include "netevents_character.h"
#include "GameWorld/player.h"
#include "Database/database_characters.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

NetEventsCharacter::NetEventsCharacter(SqliteConnection &db, CharacterManager &character_manager)
: character_manager(character_manager), db(db)
{
	netevents.func_event(CTS_CHARACTER_GETLIST).set(this, &NetEventsCharacter::on_net_event_character_get_list);
	netevents.func_event(CTS_CHARACTER_LOGIN).set(this, &NetEventsCharacter::on_net_event_character_login);
	netevents.func_event(CTS_CHARACTER_LOGOUT).set(this, &NetEventsCharacter::on_net_event_character_logout);
	netevents.func_event(CTS_CHARACTER_CREATE).set(this, &NetEventsCharacter::on_net_event_character_create);
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

bool NetEventsCharacter::dispatch_net_event(const NetGameEvent &event, Player *player)
{
	return netevents.dispatch(event, player);
}

/////////////////////////////////////////////////////////////////////////////
// Events:

void NetEventsCharacter::on_net_event_character_get_list(const NetGameEvent &e, Player *player)
{
	auto characters = DatabaseCharacters::get_available_characters(db, player->get_user_id());

	NetGameEvent event(STC_CHARACTER_LIST);

	unsigned int character_count = characters.size();
	event.add_argument(character_count);
	for(size_t i = 0; i < character_count; ++i)
	{
		event.add_argument(characters[i].id);
		event.add_argument(characters[i].name);
	}

	player->send_event(event);
}

void NetEventsCharacter::on_net_event_character_login(const NetGameEvent &e, Player *player)
{
	int requested_character_id = e.get_argument(0);

	character_manager.login_character(requested_character_id, player);
}

void NetEventsCharacter::on_net_event_character_logout(const NetGameEvent &e, Player *player)
{
	player->send_event(NetGameEvent(STC_CHARACTER_LOGOUT, "By request"));
}

void NetEventsCharacter::on_net_event_character_create(const NetGameEvent &e, Player *player)
{
	std::string character_name = e.get_argument(0);

	if(StringHelp::trim(character_name).length() == 0)
	{
		player->send_event(NetGameEvent(STC_CHARACTER_CREATE_FAIL, "Enter a username"));
		return;
	}

	if(DatabaseCharacters::is_character_name_available(db, character_name) == false)
	{
		player->send_event(NetGameEvent(STC_CHARACTER_CREATE_FAIL, "Username not available"));
		return;
	}

	int character_id = character_manager.create_character(player->get_user_id(), character_name);

	player->send_event(NetGameEvent(STC_CHARACTER_CREATE_SUCCESS, character_id));
}
