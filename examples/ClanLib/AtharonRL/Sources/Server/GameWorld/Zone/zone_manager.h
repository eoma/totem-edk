#pragma once

class Zone;
class Player;
class ServerGameObject;

class ZoneManager
{
public:
	ZoneManager(clan::SqliteConnection &db);
	~ZoneManager();

	ServerGameObject *find_gameobject(int gameobject_id) const;

	Zone *get_or_load_zone(int zone_id);

	void update();

	bool dispatch_net_event(const clan::NetGameEvent &event, Player *player);

private:
	Zone *find_zone(int zone_id);
	Zone *load_zone(int zone_id);
	Zone *instansiate_zone();

	void on_net_event_object_event(const clan::NetGameEvent &e, Player *player);

	clan::SqliteConnection &db;

	std::vector<Zone *> zones;

	clan::NetGameEventDispatcher_v1<Player *> netevents;
};
