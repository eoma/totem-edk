
#include "precomp.h"
#include "database_characters.h"

using namespace clan;

// USED TABLES: Characters

bool DatabaseCharacters::is_character_name_available(SqliteConnection &db, const std::string &user_name)
{
	DBCommand command = db.create_command("SELECT COUNT(*) FROM Characters WHERE Name LIKE ?1");
	command.set_input_parameter_string(1, user_name);

	int count = db.execute_scalar_int(command);
	return (count == 0);
}

DatabaseCharacters::CharacterInfo DatabaseCharacters::get_info(SqliteConnection &db, int character_id)
{
	if(character_id == 0)
		throw Exception("Lacking character_id");

	DBCommand command = db.create_command("SELECT Name, GameObjectId FROM Characters WHERE CharacterId=?1");
	command.set_input_parameter_int(1, character_id);

	DBReader reader = db.execute_reader(command);
	if (reader.retrieve_row())
	{
		CharacterInfo character;
		character.id = character_id;
		character.name = reader.get_column_string(0);
		character.gameobject_id = reader.get_column_int(1);

		return character;
	}

	throw Exception("Unable to retrieve character " + character_id);
}

std::vector<DatabaseCharacters::AvailableCharacterInfo> DatabaseCharacters::get_available_characters(SqliteConnection &db, int user_id)
{
	if(user_id == 0)
		throw Exception("Lacking user_id");

	std::vector<AvailableCharacterInfo> characters;

	DBCommand command = db.create_command("SELECT Characters.CharacterId, Characters.Name FROM Characters WHERE UserId=?1");
	command.set_input_parameter_int(1, user_id);

	DBReader reader = db.execute_reader(command);
	while (reader.retrieve_row())
	{
		AvailableCharacterInfo character;
		character.id = reader.get_column_int(0);
		character.name = reader.get_column_string(1);
		characters.push_back(character);
	}

	return characters;
}

void DatabaseCharacters::set_gameobject(SqliteConnection &db, int character_id, int gameobject_id)
{
	if(character_id == 0)
		throw Exception("Lacking character_id");

	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");

	DBCommand command = db.create_command("UPDATE Characters SET GameObjectId=?2 WHERE CharacterId=?1");
	command.set_input_parameter_int(1, character_id);
	command.set_input_parameter_int(2, gameobject_id);
	db.execute_non_query(command);
}

int DatabaseCharacters::create(SqliteConnection &db, int user_id, const std::string &character_name)
{
	if(character_name.length() == 0)
		throw Exception("Lacking character_name");

	if(user_id == 0)
		throw Exception("Lacking user_id");

	DBCommand command = db.create_command("INSERT INTO Characters (UserId, Name) VALUES (?1, ?2)");
	command.set_input_parameter_int(1, user_id);
	command.set_input_parameter_string(2, character_name);

	db.execute_non_query(command);

	int character_id = command.get_output_last_insert_rowid();

	return character_id;
}
