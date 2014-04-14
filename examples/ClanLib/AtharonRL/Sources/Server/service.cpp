
#include "precomp.h"
#include "service.h"
#include "server.h"

using namespace clan;

GameService::GameService()
: Service("GameServer")
{
}

void GameService::service_start(std::vector<std::string> &args)
{
	thread.start<GameService, std::string>(this, &GameService::worker_main, args[0]);
}

void GameService::service_stop()
{
	stop_event.set();
	thread.join();
}

void GameService::service_reload()
{
}

void GameService::worker_main(std::string arg)
{
	ConsoleLogger logger;
	// FileLogger fileLogger("server.log");

	Server server;
	server.run(stop_event);
}
