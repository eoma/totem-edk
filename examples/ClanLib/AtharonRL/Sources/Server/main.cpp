#include "precomp.h"
#include "service.h"

int main(int argc, char **argv)
{
	try
	{
		clan::SetupCore setup_core;
		clan::SetupNetwork setup_network;

		GameService service;

		if(argc == 1)
		{
			//Default to console mode for Service
			char *debug_argv[2] = { argv[0], "-debug" };
			return service.main(2, debug_argv);
		}
		else
		{
			return service.main(argc, argv);
		}
	}
	catch( clan::Exception &e )
	{
		clan::Console::write_line("Initialization exception: " + e.get_message_and_stack_trace());
		return -1;
	}
}
