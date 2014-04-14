
/*Totem EDK
Copyright (c) 2009 Pål Trefall and Kenneth Gangstø

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Note: Some of the libraries Totem EDK may link to may have additional
requirements or restrictions.
*/

#include "Engine.h"

#include <libtcod.hpp>
#include <string>
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif

int main(int argc, char** argv)
{
#ifdef WIN32
#ifndef _DEBUG
	auto hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);
#endif
#endif

	std::string resource_dir = argv[0];
	resource_dir = resource_dir.substr(0, resource_dir.find_last_of("\\"));
	resource_dir = resource_dir.substr(0, resource_dir.find_last_of("\\"));
	resource_dir += "\\resources\\Standard\\Roguelike\\";

	auto engine = Engine::getSingleton();
	engine->init(resource_dir, 80,43);
	
	engine->load();
	while( !TCODConsole::isWindowClosed() )
	{
		engine->update();
		engine->render();
		TCODConsole::flush();
	}
	engine->save();

	//system("pause");
	return 0;
}
