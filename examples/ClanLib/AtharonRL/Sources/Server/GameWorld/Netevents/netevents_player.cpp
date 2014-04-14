#include "precomp.h"
#include "netevents_player.h"
#include "GameWorld/player.h"
#include "GameWorld/character.h"
#include "GameWorld/server_gameobject.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

/////////////////////////////////////////////////////////////////////////////
// Construction:

NetEventsPlayer::NetEventsPlayer()
{
	netevents.func_event(CTS_PLAYER_MOVE).set(this, &NetEventsPlayer::on_net_event_player_move);
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

bool NetEventsPlayer::dispatch_net_event(const NetGameEvent &event, Player *player)
{
	return netevents.dispatch(event, player);
}

/////////////////////////////////////////////////////////////////////////////
// Events:

void NetEventsPlayer::on_net_event_player_move(const NetGameEvent &e, Player *player)
{
	auto delta_move = Vec2i(e.get_argument(0), e.get_argument(1));

	auto go = player->get_character()->get_owner_gameobject();

	if(go->hasProperty("Position"))
	{
		auto position = go->get<Vec2i>("Position");
		position += delta_move;
	}
}

