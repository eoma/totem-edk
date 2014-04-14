
#include "precomp.h"
#include "database_zone_instances.h"

using namespace clan;

// USED TABLES: ZoneInstances

void DatabaseZoneInstances::create(SqliteConnection &db, int gameobject_container_id, int generation_seed)
{
	DBTransaction transaction = db.begin_transaction();

	DBCommand command = db.create_command("INSERT INTO ZoneInstances (GameObjectContainerId, GenerationSeed) VALUES (?1, ?2)");
	command.set_input_parameter_int(1, gameobject_container_id);
	command.set_input_parameter_int(2, generation_seed);

	db.execute_non_query(command);

	transaction.commit();
}

DatabaseZoneInstances::ZoneInstanceInfo DatabaseZoneInstances::get_info(SqliteConnection &db, int gameobject_container_id)
{
	if(gameobject_container_id == 0)
		throw Exception("Lacking gameobject_container_id");

	DBCommand command = db.create_command("SELECT GenerationSeed FROM ZoneInstances WHERE GameObjectContainerId=?1");
	command.set_input_parameter_int(1, gameobject_container_id);

	DBReader reader = db.execute_reader(command);
	if (reader.retrieve_row())
	{
		ZoneInstanceInfo zone_instance;
		zone_instance.generation_seed = reader.get_column_int(0);
		zone_instance.gameobject_container_id = gameobject_container_id;

		return zone_instance;
	}

	throw Exception("Unable to retrieve zone " + gameobject_container_id);
}

bool DatabaseZoneInstances::is_zone_instantiated(SqliteConnection &db, int gameobject_container_id)
{
	DBCommand command = db.create_command("SELECT COUNT(*) FROM ZoneInstances WHERE GameObjectContainerId=?1");
	command.set_input_parameter_int(1, gameobject_container_id);

	int result = db.execute_scalar_int(command);
	return (result != 0);
}
