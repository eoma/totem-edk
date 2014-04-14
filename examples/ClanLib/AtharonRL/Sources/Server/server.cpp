#include "precomp.h"
#include "server.h"
#include "server_user.h"
#include "GameWorld/player.h"
#include "Engine/Common/Network/netevents.h"
#include "Database/database_users.h"

using namespace clan;

Server::Server()
: db("Database/game.db"), zone_manager(db), player_manager(db), character_manager(db, zone_manager), netevents_character(db, character_manager)
{
}

Server::~Server()
{
	// TODO: Delete server users
}

void Server::run(Event &stop_event)
{
	try
	{
		cl_log_event("Info", "Starting service");

		initialize_network();

		// Preload a zone for testing
		zone_manager.get_or_load_zone(1);

		cl_log_event("Info", "Server initialized and running");

		while (!stop_event.wait(0))
		{
			try
			{
				zone_manager.update();

				KeepAlive::process(10);

				System::sleep(10);
			}
			catch (Exception &e)
			{
				cl_log_event("Exception", e.get_message_and_stack_trace());
			}
		}

		network.stop();
	}
	catch (Exception &e)
	{
		cl_log_event("Exception", e.get_message_and_stack_trace());
	}

	cl_log_event("Info", "Shutting down server");
}

void Server::initialize_network()
{
	network.start("4556");

	slots.connect(network.sig_client_connected(), this, &Server::on_client_connected);
	slots.connect(network.sig_client_disconnected(), this, &Server::on_client_disconnected);
	slots.connect(network.sig_event_received(), this, &Server::on_net_event_received);

	netevents.func_event(CTS_LOGIN).set(this, &Server::on_net_event_login);
	netevents.func_event(CTS_PING).set(this, &Server::on_net_event_ping);

	cl_log_event("Network", "Network initialized");
}

void Server::on_client_connected(NetGameConnection *connection)
{
	cl_log_event("Network", "Client connected");

	// Create user and attach it to connection
	/*ServerUser *user =*/ new ServerUser(connection);

	std::string html =
		"<html>"
		"<body>"
		"<h1>Urgent Realm Maintenance</h1>"
		"<div class='time'>2011/12/21 05:47:00 PM</div>"
		"<div>We'll be performing urgent rolling restarts on all realms beginning at 10:00 a.m. PST, with an additional hour of downtime for the realms listed below. The rolling restarts are expected to impact each realm for approximately 15 minutes once they begin, and the realms below are expected to be available again by approximately 11:00 a.m. PST. We apologize for the short notice and appreciate your patience.</div>"
		"<ul>"
		"<li>Antonidas</li>"
		"<li>Anub'arak</li>"
		"<li>Blackwater Raiders</li>"
		"<li>Bladefist</li>"
		"<li>Borean Tundra</li>"
		"<ul>"
		"</body>"
		"</html>";

	connection->send_event(NetGameEvent(STC_MOTD, html));
}

void Server::on_client_disconnected(NetGameConnection *connection)
{
	cl_log_event("Network", "Client disconnected");

	ServerUser *user = ServerUser::get_user(connection);

	if(user)
	{
		if(user->is_logged_in())
		{
			player_manager.remove_player(connection);
		}

		delete user;
	}
}

void Server::on_net_event_received(NetGameConnection *connection, const NetGameEvent &e)
{
	cl_log_event("Network", "Client sent event: %1", e.to_string());

	ServerUser *user = ServerUser::get_user(connection);
	if(user)
	{
		bool handled_event = netevents.dispatch(e, user);

		if (user->is_logged_in())
		{
			Player *player = player_manager.find_player(connection);
			if(player)
			{	
				cl_log_event("Network", "Player: %1", player->get_user_id());

				handled_event |= netevents_player.dispatch_net_event(e, player);
				handled_event |= netevents_character.dispatch_net_event(e, player);
				handled_event |= zone_manager.dispatch_net_event(e, player);
			}
		}

		if (!handled_event)
			cl_log_event("Network", "Unhandled event: %1", e.to_string());
	}
}

void Server::on_net_event_login(const NetGameEvent &e, ServerUser *user)
{
	std::string user_name = e.get_argument(0);
	std::string password = e.get_argument(1);

	int user_id = DatabaseUsers::login(db, user_name, password);
	if(user_id)
	{
		try
		{
			player_manager.create_player(user_id, user->get_connection());

			user->set_logged_in();
			user->send_event(NetGameEvent(STC_LOGIN_SUCCESS));

			cl_log_event("Network", "Client logged in: " + user_name);
		}
		catch(Exception ex)
		{
			user->send_event(NetGameEvent(STC_LOGIN_FAIL, "Failed to login"));

			cl_log_event("System", "Failed to login: " + ex.message);
		}
	}
	else
	{
		user->send_event(NetGameEvent(STC_LOGIN_FAIL, "Wrong username or password"));

		cl_log_event("Network", "Client login failed: Wrong username or password");
	}
}

void Server::on_net_event_ping(const NetGameEvent &e, ServerUser *user)
{
	int time = e.get_argument(0);
	user->send_event(NetGameEvent(STC_PONG, time));
}
