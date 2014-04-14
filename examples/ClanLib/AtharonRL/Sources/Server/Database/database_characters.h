#pragma once

class DatabaseCharacters
{
public:
	struct AvailableCharacterInfo
	{
		int id;
		std::string name;
	};

	struct CharacterInfo
	{
		int id;
		std::string name;
		int gameobject_id;
	};

public:
	static bool is_character_name_available(clan::SqliteConnection &db, const std::string &user_name);

	static CharacterInfo get_info(clan::SqliteConnection &db, int character_id);

	static std::vector<AvailableCharacterInfo> get_available_characters(clan::SqliteConnection &db, int user_id);

	static void set_gameobject(clan::SqliteConnection &db, int character_id, int gameobject_id);

	static int create(clan::SqliteConnection &db, int user_id, const std::string &character_name);
};
