
#pragma once

#include "Engine/Client/UIFramework/ui_screen_manager.h"
#include "Engine/Client/UIFramework/ui_screen.h"
#include "Engine/Client/Scene/layer_manager.h"
#include "GameWorld/client_zone.h"

class Game;

class GameScreen /*: public UIScreen*/
{
public:
	GameScreen(UIScreenManager *screen_manager, Game *game, clan::NetGameClient &network, clan::ResourceManager &resources);

	void update();
	void draw(clan::Canvas &canvas);


private:
	void on_activated();

	void on_event_received(const clan::NetGameEvent &e);
	void on_key_down(const clan::InputEvent &event);

	void move(int delta_x, int delta_y);

	Game *game;

	bool is_awaiting_response;

	std::string css_text;

	clan::NetGameClient &network;
	clan::SlotContainer slots;

	std::shared_ptr<LayerManager> layer_manager;
	std::shared_ptr<Layer> dungeon_layer;
	std::shared_ptr<Layer> character_layer;

	std::unique_ptr<ClientZone> zone;
};
