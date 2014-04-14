#include "precomp.h"
#include "character_manager.h"
#include "character.h"
#include "player.h"
#include "Zone/zone.h"
#include "Engine/Common/Network/netevents.h"
#include "Database/database_characters.h"
#include "Database/database_gameobjects.h"

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

CharacterManager::CharacterManager(SqliteConnection &db, ZoneManager &zone_manager)
: db(db), zone_manager(zone_manager)
{
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CharacterManager::login_character(int requested_character_id, Player *player)
{
	std::vector<DatabaseCharacters::AvailableCharacterInfo> characters = DatabaseCharacters::get_available_characters(db, player->get_user_id());
	for(size_t i = 0; i < characters.size(); ++i)
	{
		if(characters[i].id == requested_character_id)
		{
			initialize_character(requested_character_id, player);
			return;
		}
	}

	player->send_event(NetGameEvent(STC_CHARACTER_LOGIN_FAIL, "Character not found"));
}

int CharacterManager::create_character(int user_id, const std::string &character_name)
{
	int character_id = DatabaseCharacters::create(db, user_id, character_name);

	return character_id;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CharacterManager::initialize_character(int character_id, Player *player)
{
	try
	{
		DatabaseCharacters::CharacterInfo character_info = DatabaseCharacters::get_info(db, character_id);
		DatabaseGameObjects::GameObjectInfo gameobject_info = DatabaseGameObjects::get_info(db, character_info.gameobject_id);

		Zone *zone = zone_manager.get_or_load_zone(gameobject_info.container_id);
		if(zone == 0)
			throw Exception(string_format("Unable to load zone %1", gameobject_info.container_id));

		DatabaseGameObjects::set_gameobject_load_state(db, character_info.gameobject_id, false);

		ServerGameObject *gameobject = zone->load_gameobject(character_info.gameobject_id);
		if(gameobject == 0)
			throw Exception(string_format("Unable to load gameobject %1", character_info.gameobject_id));

		player->send_event(NetGameEvent(STC_CHARACTER_LOGIN_SUCCESS));

		Character *character = player->create_character(character_info.id, character_info.name, gameobject);

		character->move_to_zone(zone, false);
	}
	catch (Exception &e)
	{
		player->send_event(NetGameEvent(STC_CHARACTER_LOGIN_FAIL, "Unable to login"));

		cl_log_event("System", string_format("Failed to login character %1 (%2)", character_id, e.message));
	}
}
