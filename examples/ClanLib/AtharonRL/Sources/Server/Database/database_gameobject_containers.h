#pragma once

class DatabaseGameObjectContainers
{
public:
	static void create(clan::SqliteConnection &db, int gameobject_container_id, const std::string &description);

	static std::vector<int> get_load_by_default_gameobject_ids_in_container(clan::SqliteConnection &db, int gameobject_container_id);
	static void move_gameobject_to_container(clan::SqliteConnection &db, int gameobject_id, int gameobject_container_id);
};
