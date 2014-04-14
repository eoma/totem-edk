#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"
#include "../Utils/Persistent.h"

#include <vector>
#include <memory>

#include <libtcod.hpp>

static const int PANEL_HEIGHT=7;
static const int BAR_WIDTH=20;
static const int MSG_X=BAR_WIDTH+2;
static const int MSG_HEIGHT=PANEL_HEIGHT-1;

class RenderSystem;
typedef std::shared_ptr<RenderSystem> RenderSystemPtr;

class Gui;
typedef std::shared_ptr<Gui> GuiPtr;

class Gui : public Totem::Component<Gui, PropertyUserData>, public Persistent
{
public:
	Gui(const EntityWPtr &owner, const RenderSystemPtr &system);
    virtual ~Gui();

	EntityPtr getOwner() { return owner.lock(); }

	void render();

	void message(const TCODColor &col, const char *text, ...);

	//Defined in Persistent.cpp
	void save(TCODZip &zip) override;
	void load(TCODZip &zip) override;

private:
	EntityWPtr owner;
	PropertyUserData user_data;
	RenderSystemPtr system;
	
	TCODConsole *con;

	class Message {
	public:
        std::string text;
        TCODColor col;
		Message(const std::string &text, const TCODColor &col) : text(text), col(col) {}
    };
	typedef std::shared_ptr<Message> MessagePtr;
    std::vector<MessagePtr> log;

	void renderBar(int x, int y, int width, const char *name,
        float value, float maxValue, const TCODColor &barColor,
        const TCODColor &backColor);

	void renderMouseLook();

	
};