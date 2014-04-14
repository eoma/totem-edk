
#include "precomp.h"
#include "database_gameobject_containers.h"

using namespace clan;

// USED TABLES: GameObjectContainers

void DatabaseGameObjectContainers::create(SqliteConnection &db, int gameobject_container_id, const std::string &description)
{
	if(gameobject_container_id == 0)
		throw Exception("Lacking gameobject_container_id");

	DBCommand command = db.create_command("INSERT INTO GameObjectContainers (GameObjectContainerId, Description) VALUES (?1, ?2)");
	command.set_input_parameter_int(1, gameobject_container_id);
	command.set_input_parameter_string(2, description);
	
	db.execute_non_query(command);
}

std::vector<int> DatabaseGameObjectContainers::get_load_by_default_gameobject_ids_in_container(SqliteConnection &db, int gameobject_container_id)
{
	if(gameobject_container_id == 0)
		throw Exception("Lacking gameobject_container_id");

	DBCommand command = db.create_command(
		"SELECT GameObjects.GameObjectId FROM GameObjects WHERE GameObjectContainerId=?1 AND LoadByDefault=1");
	command.set_input_parameter_int(1, gameobject_container_id);

	std::vector<int> gameobject_id_list;

	DBReader reader = db.execute_reader(command);
	while (reader.retrieve_row())
	{
		gameobject_id_list.push_back(reader.get_column_int(0));
	}

	return gameobject_id_list;
}

void DatabaseGameObjectContainers::move_gameobject_to_container(SqliteConnection &db, int gameobject_id, int gameobject_container_id)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id id");

	DBCommand command = db.create_command("UPDATE GameObjects SET GameObjectContainerId=?2 WHERE GameObjectId=?1");
	command.set_input_parameter_int(1, gameobject_id);
	command.set_input_parameter_int(2, gameobject_container_id);

	db.execute_non_query(command);
}