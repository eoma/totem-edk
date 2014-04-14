
#include "precomp.h"
#include "program.h"
#include "redballapp.h"
#include <ClanLib/application.h>

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	try
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_SetupSound setup_sound;
		CL_SetupVorbis setup_vorbis;
		CL_SoundOutput output(44100);

		RedBallApp app;
		app.run();
	}
	catch (CL_Exception e)
	{
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + e.get_message_and_stack_trace());
		console.display_close_message();
		return -1;
	}

	return 0;
}
