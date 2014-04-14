
#include "precomp.h"
#include "program.h"
#include "game.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	try
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;
		SetupGUI setup_gui;
		SetupNetwork setup_network;
		SetupSound setup_sound;
		SetupVorbis setup_vorbis;

		srand(System::get_time());

		ConsoleLogger logger;

		std::string username = "sphair";
		std::string password = "p";
		int character = 1;

		if(args.size() > 1)
			username = args[1];
		if(args.size() > 2)
			password = args[2];
		if(args.size() > 3)
			character = StringHelp::text_to_int(args[3]);

		Game game(username, password, character);
		game.run();

		return 0;
	}
	catch (Exception &e)
	{
		ConsoleWindow console("Console", 80, 160);
		Console::write_line("Exception caught: " + e.get_message_and_stack_trace());
		console.display_close_message();
		return -1;
	}
}
