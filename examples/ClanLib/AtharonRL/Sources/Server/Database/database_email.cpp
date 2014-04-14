
#include "precomp.h"
#include "database_email.h"

using namespace clan;

// USED TABLES: Email

std::vector<DatabaseEmail::EmailInfo> DatabaseEmail::get_emails_for_character(SqliteConnection &db, int character_id)
{
	if(character_id == 0)
		throw Exception("Lacking character_id");

	std::vector<EmailInfo> emails;

	DBCommand command = db.create_command("SELECT EmailId, CreateDate, SenderCharacterId, SenderName, IsRead, Folder, Subject, Message FROM Emails WHERE ReceiverCharacterId=?1");
	command.set_input_parameter_int(1, character_id);

	DBReader reader = db.execute_reader(command);
	while (reader.retrieve_row())
	{
		EmailInfo email;
		email.receiver_character_id = character_id;
		email.email_id = reader.get_column_int(0);
		email.create_date = reader.get_column_datetime(1);
		email.sender_character_id = reader.get_column_int(2);
		email.sender_name = reader.get_column_string(3);
		email.is_read = reader.get_column_bool(4);
		email.folder = reader.get_column_string(5);
		email.subject = reader.get_column_string(6);
		email.message = reader.get_column_string(7);

		emails.push_back(email);
	}

	return emails;
}

int DatabaseEmail::create(SqliteConnection &db, int receiver_character_id, int sender_character_id, const std::string &sender_name, const std::string &folder, const std::string &subject, const std::string &message)
{
	if(subject.length() == 0)
		throw Exception("Email lacking subject");
	if(message.length() == 0)
		throw Exception("Email lacking message");
	if(receiver_character_id == 0)
		throw Exception("Email lacking receiver_character_id");
	if(sender_name.length() == 0)
		throw Exception("Email lacking sender_name");
	if(folder.length() == 0)
		throw Exception("Email lacking folder");

	DBCommand command = db.create_command("INSERT INTO Emails (ReceiverCharacterId, SenderCharacterId, SenderName, Folder, Subject, Message) VALUES (?1, ?2, ?3, ?4, ?5, ?6)");
	command.set_input_parameter_int(1, receiver_character_id);
	command.set_input_parameter_int(2, sender_character_id);
	command.set_input_parameter_string(3, sender_name);
	command.set_input_parameter_string(4, folder);
	command.set_input_parameter_string(5, subject);
	command.set_input_parameter_string(6, message);

	db.execute_non_query(command);

	int email_id = command.get_output_last_insert_rowid();
	return email_id;
}
