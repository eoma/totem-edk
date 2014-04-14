#pragma once

class DatabaseGameObjects
{
public:
	struct GameObjectInfo
	{
		int gameobject_id;
		int container_id;
		bool load_by_default;
		std::string description;
	};

	struct GameObjectPropertyInfo
	{
		int type_id;
		std::string name;
		std::string value;
	};

	struct GameObjectComponentInfo
	{
		std::string type;
		std::string name;
	};

	struct GameObjectDirtyPropertyInfo
	{
		int gameobject_id;
		std::string component_type;
		std::string component_name;
		std::string property_name;
		std::string property_value;
	};

public:
	static GameObjectInfo get_info(clan::SqliteConnection &db, int gameobject_id);

	static std::vector<GameObjectComponentInfo> get_components(clan::SqliteConnection &db, int gameobject_id);
	static std::vector<GameObjectPropertyInfo> get_properties(clan::SqliteConnection &db, int gameobject_id);

	static void create(clan::SqliteConnection &db, int gameobject_id, const std::string &description);

	static void save_dirty_properties(clan::SqliteConnection &db, std::vector<GameObjectDirtyPropertyInfo> &properties);

	static void add_component(clan::SqliteConnection &db, int gameobject_id, const std::string &component_type, const std::string &component_name);
	static void remove_component(clan::SqliteConnection &db, int gameobject_id, const std::string &component_type, const std::string &component_name);

	static void add_property(clan::SqliteConnection &db, int gameobject_id, int property_type, const std::string &property_name, const std::string &property_value);
	static void remove_property(clan::SqliteConnection &db, int gameobject_id, const std::string &property_name);

	static void set_gameobject_load_state(clan::SqliteConnection &db, int gameobject_id, bool load_by_default);
};
