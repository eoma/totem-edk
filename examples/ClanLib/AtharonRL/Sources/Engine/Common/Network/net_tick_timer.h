
#pragma once

#include <list>

class ScriptNetObject;

class NetTickTimer
{
public:
	NetTickTimer();

	virtual void start(std::string hostname, std::string port) = 0;
	virtual void stop() = 0;

	virtual void update() = 0;

	clan::NetGameEvent create_event(const std::string &name);
	virtual void send_net_event(const std::string &target, const clan::NetGameEvent &net_event) = 0;

	virtual std::string create_id() = 0;
	//void add_net_object(const std::string &obj_id, ScriptNetObject *obj);
	//void remove_net_object(const std::string &obj_id);

protected:
	int ticks_elapsed();
	void step_simulation(int local_tick_time, int server_arrival_tick_time);

	static const int ticks_per_second = 60;

	struct QueuedMessage
	{
		QueuedMessage(std::string sender, clan::NetGameEvent netevent) : sender(sender), netevent(netevent) { }
		std::string sender;
		clan::NetGameEvent netevent;
	};
	std::list<QueuedMessage> received_messages;

private:
	unsigned int start_time;
	unsigned int last_tick;

	int server_arrival_tick_time;
};

class NetTickTimerClient : public NetTickTimer
{
public:
	NetTickTimerClient();

	void start(std::string hostname, std::string port);
	void stop();

	void update();

	void send_net_event(const std::string &target, const clan::NetGameEvent &net_event);
	std::string create_id() { throw clan::Exception("Client cannot create new net objects"); }

	static int actual_ping;

private:
	void on_connected();
	void on_disconnected();
	void on_event_received(const clan::NetGameEvent &net_event);

	clan::NetGameClient netgame;
	clan::SlotContainer slots;

	int server_tick_time;
	int client_tick_time;
	int ping;
	int jitter;
	bool skipped_last_tick;
};

class NetTickTimerServer : public NetTickTimer
{
public:
	NetTickTimerServer();

	void start(std::string hostname, std::string port);
	void stop();

	void update();

	void send_net_event(const std::string &target, const clan::NetGameEvent &net_event);
	std::string create_id();

private:
	void on_client_connected(clan::NetGameConnection *connection);
	void on_client_disconnected(clan::NetGameConnection *connection);
	void on_event_received(clan::NetGameConnection *connection, const clan::NetGameEvent &net_event);

	struct ConnectionData
	{
		ConnectionData(std::string id) : id(id), ping(0) { }
		std::string id;
		unsigned int ping;
		std::list<clan::NetGameEvent> events;
	};

	clan::NetGameServer netgame;
	clan::SlotContainer slots;
	std::map<std::string, clan::NetGameConnection *> clients;
	int next_client_id;
	int next_obj_id;

	int tick_time;
};
