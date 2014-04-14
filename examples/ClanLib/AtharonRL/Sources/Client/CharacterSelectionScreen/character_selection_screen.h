
#pragma once

#include "Engine/Client/UIFramework/ui_screen_manager.h"
#include "Engine/Client/UIFramework/ui_screen.h"

class UIFrame;
class UIButton;
class UILineEdit;
class UILabel;
class Game;
class Layer;

class CharacterSelectionScreen : public UIScreen
{
public:
	CharacterSelectionScreen(UIScreenManager *screen_manager, Game *game, clan::NetGameClient &network, clan::ResourceManager &resources);

	void update();

private:
	virtual void on_activated();
	void on_create_new_clicked();
	void on_select_character(size_t i);
	void on_join_clicked();
	void on_event_received(const clan::NetGameEvent &e);
	void on_event_character_list(const clan::NetGameEvent &e);
	void on_event_character_login_success(const clan::NetGameEvent &e);
	void on_event_character_login_fail(const clan::NetGameEvent &e);
	void on_event_character_logout(const clan::NetGameEvent &e);
	void on_event_character_create_success(const clan::NetGameEvent &e);
	void on_event_character_create_fail(const clan::NetGameEvent &e);

	void add_character(int id, const std::string &name);

	Game *game;
	clan::NetGameClient &network;
	clan::SlotContainer slots;
	clan::NetGameEventDispatcher_v0 netevents;
	clan::Image background;

	UIButton *button_create_new;
	std::vector<UIFrame *> frame_character;
	std::vector<UIButton *> button_character;
	std::vector<int> button_character_ids;
	size_t current_character;
	UIButton *button_join;
	UILabel *label_status;
};
