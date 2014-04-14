#include "Gui.h"
#include "../Engine.h"
#include "Actor.h"
#include "../Systems/RenderSystem.h"

#include <iostream>

Gui::Gui(const EntityWPtr &owner, const RenderSystemPtr &system) 
: Totem::Component<Gui, PropertyUserData>("Gui"), owner(owner), system(system)
{
	user_data.entity = owner;
	user_data.component = this;

	con = new TCODConsole(Engine::getSingleton()->getScreenWidth(), PANEL_HEIGHT);

	system->set(this);
}

Gui::~Gui()
{
	//std::cout << "Gui is being destroyed!" << std::endl;
	delete con;

	system->set(static_cast<Gui*>(nullptr));
}

void Gui::render()
{
	auto engine = Engine::getSingleton();
	auto player = engine->getPlayer();

	// clear the GUI console
    con->setDefaultBackground(TCODColor::black);
    con->clear();

	// mouse look
	renderMouseLook();

	// draw the health bar
	renderBar(1,1,BAR_WIDTH,"HP",player->getOwner()->get<float>("HP").get(), player->getOwner()->get<float>("MaxHP").get(), TCODColor::lightRed,TCODColor::darkerRed);

	// draw the message log
	int y=1;
	float colorCoef=0.4f;
	for (unsigned int i = 0; i < log.size(); i++)
	{
		auto message = log[i];
		con->setDefaultForeground(message->col * colorCoef);
		con->print(MSG_X,y,message->text.c_str());
		y++;
		if ( colorCoef < 1.0f )
			colorCoef+=0.3f;
	}

	// blit the GUI console on the root console
	TCODConsole::blit(con,0,0,engine->getScreenWidth(),PANEL_HEIGHT, TCODConsole::root,0,engine->getScreenHeight()-PANEL_HEIGHT);
}

void Gui::renderBar(int x, int y, int width, const char *name, float value, float maxValue, const TCODColor &barColor, const TCODColor &backColor) 
{
	// fill the background
	con->setDefaultBackground(backColor);
	con->rect(x,y,width,1,false,TCOD_BKGND_SET);

	int barWidth = (int)(value / maxValue * width);
	if ( barWidth > 0 ) 
	{
		// draw the bar
		con->setDefaultBackground(barColor);
		con->rect(x,y,barWidth,1,false,TCOD_BKGND_SET);
	}

	// print text on top of the bar
    con->setDefaultForeground(TCODColor::white);
    con->printEx(x+width/2,y,TCOD_BKGND_NONE,TCOD_CENTER, "%s : %g/%g", name, value, maxValue);
}

void Gui::renderMouseLook() 
{
	auto engine = Engine::getSingleton();
	auto map = engine->getMap();
	auto mouse_pos = Vec2i(engine->getMouse().cx, engine->getMouse().cy);
	if (!map->isInFov(mouse_pos)) 
	{
        // if mouse is out of fov, nothing to render
        return;
    }

	std::string buf;

	bool first=true;
	auto actors = engine->getActors();
	for (unsigned int i = 0; i < actors.size(); i++) 
	{
		auto actor = actors[i];
		// find actors under the mouse cursor
		if (actor->getPosition() == mouse_pos) 
		{
			if(!first)	buf+=", ";
			else		first=false;

			buf += actor->getOwner()->getName();
		}
	}

	// display the list of actors under the mouse cursor
    con->setDefaultForeground(TCODColor::lightGrey);
	con->print(1,0,buf.c_str());
}

void Gui::message(const TCODColor &col, const char *text, ...)
{
    // build the text
    va_list ap;
    char buf[128];
    va_start(ap,text);
    vsprintf(buf,text,ap);
    va_end(ap);

	char *lineBegin=buf;
	char *lineEnd;

	do 
	{
		// make room for the new message
		if ( log.size() == MSG_HEIGHT )
			log.erase(log.begin());

		// detect end of the line
		lineEnd=strchr(lineBegin,'\n');
		if(lineEnd)
			*lineEnd='\0';

		// add a new message to the log
		log.push_back(std::make_shared<Message>(lineBegin, col));

		// go to next line
        lineBegin=lineEnd+1;
	} while ( lineEnd );
}