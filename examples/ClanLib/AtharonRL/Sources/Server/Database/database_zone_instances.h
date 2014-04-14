#pragma once

class DatabaseZoneInstances
{
public:
	struct ZoneInstanceInfo
	{
		int gameobject_container_id;
		int generation_seed;
	};

public:
	static void create(clan::SqliteConnection &db, int gameobject_container_id, int generation_seed);

	static ZoneInstanceInfo get_info(clan::SqliteConnection &db, int gameobject_container_id);

	static bool is_zone_instantiated(clan::SqliteConnection &db, int gameobject_container_id);
};
