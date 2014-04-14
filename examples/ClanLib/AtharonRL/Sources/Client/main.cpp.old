#include "precomp.h"
#include "game.h"

int main(int argc, char **argv)
{
	try
	{
		clan::SetupCore setup_core;
		clan::SetupNetwork setup_network;
		//clan::SetupVorbis setup_vorbis;

		srand(clan::System::get_time());

		std::string base_dir = argv[0];
		base_dir = base_dir.substr(0, base_dir.find_last_of("\\"));
		base_dir = base_dir.substr(0, base_dir.find_last_of("\\"));
		base_dir = base_dir.substr(0, base_dir.find_last_of("\\"));

		clan::FileLogger logger(base_dir+"/log.txt");
		clan::ConsoleLogger logger2;

		Game game(base_dir);
		game.run();

		return 0;
	}
	catch( clan::Exception &e )
	{
		clan::Console::write_line("Exception caught: " + e.get_message_and_stack_trace());
		return -1;
	}
}