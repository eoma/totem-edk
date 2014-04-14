#pragma once

class DatabaseEmail
{
public:
	struct EmailInfo
	{
		int email_id;
		clan::DateTime create_date;
		int receiver_character_id;
		int sender_character_id;
		std::string sender_name;
		bool is_read;
		std::string folder;
		std::string subject;
		std::string message;
	};

public:
	static std::vector<EmailInfo> get_emails_for_character(clan::SqliteConnection &db, int character_id);

	static int create(clan::SqliteConnection &db, int receiver_character_id, int sender_character_id, const std::string &sender_name, const std::string &folder, const std::string &subject, const std::string &message);
};
