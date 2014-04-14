#include "precomp.h"
#include "character.h"
#include "player.h"
#include "server_gameobject.h"
#include "Zone/zone.h"
#include "Database/database_characters.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

Character::Character(SqliteConnection &db, int character_id, const std::string &character_name, Player *player, ServerGameObject *gameobject)
: db(db), character_id(character_id), character_name(character_name), owner_gameobject(0), zone(0), player(player)
{
	set_owner_gameobject(gameobject);
}

Character::~Character()
{
	remove_from_zone();
}

void Character::set_owner_gameobject(ServerGameObject *gameobject)
{
	owner_gameobject = gameobject;

	if(gameobject)
	{
		NetGameEvent event(STC_OBJECT_PLAYER_OWN, gameobject->get_id());
		player->send_event(event);
	}
}

void Character::remove_from_zone()
{
	if(zone)
	{
		zone->remove_player(player);
		zone->set_gameobject_inactive(owner_gameobject);
		
		owner_gameobject->save_dirty_properties();

		zone = 0;
	}
}

void Character::move_to_zone(Zone *new_zone, bool set_start_position)
{
	if(owner_gameobject == 0)
		throw Exception("move_to_zone called without setting gameobject first");

	if(zone != new_zone)
	{
		remove_from_zone();

		if(set_start_position)
		{
			// Get start position somehow
			Vec2i start_pos = Vec2i(0,0);
			owner_gameobject->get<Vec2i>("Position") = start_pos;
		}

		zone = new_zone;
		zone->add_player(player);
		zone->add_gameobject(owner_gameobject);
	}
}
