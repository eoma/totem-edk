
#pragma once

class GameService : public clan::Service
{
public:
	GameService();

	void service_start(std::vector<std::string> &args);
	void service_stop();
	void service_reload();

private:
	void worker_main(std::string arg);

	clan::Thread thread;
	clan::Event stop_event;
};
