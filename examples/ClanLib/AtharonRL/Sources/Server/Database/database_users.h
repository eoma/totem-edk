#pragma once

class DatabaseUsers
{
public:
	struct UserInfo
	{
		int user_id;
		std::string user_name;
		std::string real_name;
		clan::DateTime last_login_date;
		clan::DateTime created_date;
	};

public:
	// Returns id of user is successful, 0 otherwise. This also updates the LastLoginDate.
	static int login(clan::SqliteConnection &db, const std::string &user_name, const std::string &password);

	// Retrieves a user with a specific user id. Throws exception if not found.
	static UserInfo get_info(clan::SqliteConnection &db, int user_id);

	// Retrieves a user with a specific user name. Throws exception if not found.
	static UserInfo get_info(clan::SqliteConnection &db, const std::string &user_name);
};
