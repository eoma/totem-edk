
#include "precomp.h"
#include "database_gameobjects.h"

using namespace clan;

// USED TABLES: GameObjects, GameObjectProperties, GameObjectComponents

DatabaseGameObjects::GameObjectInfo DatabaseGameObjects::get_info(SqliteConnection &db, int gameobject_id)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");

	DBCommand command = db.create_command("SELECT GameObjectContainerId, LoadByDefault, Description FROM GameObjects WHERE GameObjectId=?1");
	command.set_input_parameter_int(1, gameobject_id);

	DBReader reader = db.execute_reader(command);
	if (reader.retrieve_row())
	{
		GameObjectInfo gameobject;
		gameobject.gameobject_id = gameobject_id;
		gameobject.container_id = reader.get_column_int(0);
		gameobject.load_by_default = reader.get_column_bool(1);
		gameobject.description = reader.get_column_string(2);

		return gameobject;
	}

	throw Exception("Unable to retrieve gameobject " + gameobject_id);
}

void DatabaseGameObjects::create(SqliteConnection &db, int gameobject_id, const std::string &description)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");

	DBCommand command = db.create_command("INSERT INTO GameObjects (GameObjectId, Description) VALUES (?1, ?2)");
	command.set_input_parameter_int(1, gameobject_id);
	command.set_input_parameter_string(2, description);

	db.execute_non_query(command);
}

void DatabaseGameObjects::save_dirty_properties(SqliteConnection &db, std::vector<GameObjectDirtyPropertyInfo> &properties)
{
	DBTransaction transaction = db.begin_transaction();

	// TODO; Handle Component properties when saving SQL

	for(size_t i = 0; i < properties.size(); ++i)
	{
		DBCommand command = db.create_command("UPDATE GameObjectProperties SET Value=?3 WHERE GameObjectId=?1 AND Name=?2");
		command.set_input_parameter_int(1, properties[i].gameobject_id);
		command.set_input_parameter_string(2, properties[i].property_name);
		command.set_input_parameter_string(3, properties[i].property_value);

		db.execute_non_query(command);
	}

	transaction.commit();
}

std::vector<DatabaseGameObjects::GameObjectComponentInfo> DatabaseGameObjects::get_components(SqliteConnection &db, int gameobject_id)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");

	DBCommand command = db.create_command(
		"SELECT "
		"   GameObjectComponents.Type, "
		"   GameObjectComponents.Name "
		"FROM "
		"   GameObjectComponents "
		"WHERE "
		"   GameObjectId=?1");
	command.set_input_parameter_int(1, gameobject_id);

	std::vector<GameObjectComponentInfo> results;

	DBReader reader = db.execute_reader(command);
	while (reader.retrieve_row())
	{
		GameObjectComponentInfo component_info;

		component_info.type = reader.get_column_string(0);
		component_info.name = reader.get_column_string(1);

		results.push_back(component_info);
	}

	return results;
}

std::vector<DatabaseGameObjects::GameObjectPropertyInfo> DatabaseGameObjects::get_properties(SqliteConnection &db, int gameobject_id)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");

	DBCommand command = db.create_command(
		"SELECT "
		"   GameObjectProperties.Type, "
		"   GameObjectProperties.Name, "
		"   GameObjectProperties.Value "
		"FROM "
		"   GameObjectProperties "
		"WHERE "
		"   GameObjectId=?1");
	command.set_input_parameter_int(1, gameobject_id);

	std::vector<GameObjectPropertyInfo> results;

	DBReader reader = db.execute_reader(command);
	while (reader.retrieve_row())
	{
		GameObjectPropertyInfo property_info;

		property_info.type_id = reader.get_column_int(0);
		property_info.name = reader.get_column_string(1);
		property_info.value = reader.get_column_string(2);

		results.push_back(property_info);
	}

	return results;
}

void DatabaseGameObjects::add_component(SqliteConnection &db, int gameobject_id, const std::string &component_type, const std::string &component_name)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");
	if(component_type.length() == 0)
		throw Exception("Lacking component_type");

	DBCommand command = db.create_command("INSERT INTO GameObjectComponents (GameObjectId, Type, Name) VALUES (?1,?2,?3)");
	command.set_input_parameter_int(1, gameobject_id);
	command.set_input_parameter_string(2, component_type);
	command.set_input_parameter_string(3, component_name);

	db.execute_non_query(command);
}

void DatabaseGameObjects::remove_component(SqliteConnection &db, int gameobject_id, const std::string &component_type, const std::string &component_name)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");
	if(component_type.length() == 0)
		throw Exception("Lacking component_type");

	DBCommand command = db.create_command("DELETE FROM GameObjectComponents Where GameObjectId=?1 AND Type=?2 AND Name=?3");
	command.set_input_parameter_int(1, gameobject_id);
	command.set_input_parameter_string(2, component_type);
	command.set_input_parameter_string(3, component_name);

	db.execute_non_query(command);
}

void DatabaseGameObjects::add_property(SqliteConnection &db, int gameobject_id, int property_type, const std::string &property_name, const std::string &property_value)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");
	if(property_type == 0)
		throw Exception("Lacking property_type");
	if(property_name.length() == 0)
		throw Exception("Lacking property_name");

	DBCommand command = db.create_command("INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (?1,?2,?3,?4)");
	command.set_input_parameter_int(1, gameobject_id);
	command.set_input_parameter_int(2, property_type);
	command.set_input_parameter_string(3, property_name);
	command.set_input_parameter_string(4, property_value);

	db.execute_non_query(command);
}

void DatabaseGameObjects::remove_property(SqliteConnection &db, int gameobject_id, const std::string &property_name)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");

	DBCommand command = db.create_command("DELETE FROM GameObjectProperties Where GameObjectId=?1 AND Name=?2");
	command.set_input_parameter_int(1, gameobject_id);
	command.set_input_parameter_string(2, property_name);

	db.execute_non_query(command);
}

void DatabaseGameObjects::set_gameobject_load_state(SqliteConnection &db, int gameobject_id, bool load_by_default)
{
	if(gameobject_id == 0)
		throw Exception("Lacking gameobject_id");

	DBCommand command = db.create_command("UPDATE GameObjects SET LoadByDefault=?2 WHERE GameObjectId=?1");
	command.set_input_parameter_int(1, gameobject_id);
	command.set_input_parameter_bool(2, load_by_default);

	db.execute_non_query(command);
}
