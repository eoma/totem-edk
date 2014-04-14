#pragma once

//TODO: Need to refactor this class substantially. Zone should really be a container of players, gameobjects and the map of a zone...
//		right now it handles way too much logic!

#include "zone_vicinity_objects.h"
#include "zone_vicinity_map.h"
#include "zone_player_manager.h"
#include "GameWorld/gameobject_container.h"
#include "GameWorld/component_factory.h"
#include "Engine/Common/GameWorld/tick_timer.h"

class Player;
class ServerGameObject;
class ZoneMap; typedef std::shared_ptr<ZoneMap> ZoneMapPtr;

class Zone
{
// Construction:
public:
	//Loading existing zone
	Zone(clan::SqliteConnection &db, int zone_id, const GameObjectContainerPtr &gameobjects, const ZoneMapPtr &map, const ComponentFactoryPtr &component_factory);
	~Zone();

// Attributes:
public:
	int get_id() const;

	ServerGameObject *find_gameobject(int gameobject_id) const;
	Player *find_player_with_gameobject(ServerGameObject *gameobject) const;



// Operations:
public:
	void add_player(Player *player);
	void remove_player(Player *player);

	void add_gameobject(ServerGameObject *gameobject);
	void remove_gameobject(ServerGameObject *gameobject);
	void set_gameobject_inactive(ServerGameObject *gameobject);

	void update();

	void send_event(const clan::NetGameEvent &event);

	ServerGameObject *load_gameobject(int gameobject_id);
	const GameObjectContainerPtr &get_gameobjects() const { return gameobjects; }

private:
	void tick(float time_elapsed);

	void notify_players_object_added(ServerGameObject *gameobject);
	void notify_players_object_removed(ServerGameObject *gameobject);
	void notify_players_map_changed();

	void save();

	ZonePlayerManager players;
	GameObjectContainerPtr gameobjects;
	ZoneMapPtr map;

	ComponentFactoryPtr component_factory;

	int zone_id;

	TickTimer tick_timer;
};
