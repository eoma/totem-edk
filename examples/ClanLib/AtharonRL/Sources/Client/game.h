
#pragma once

#include "Engine/Client/UIFramework/ui_screen_manager.h"
#include "Engine/Client/Audio/music_player.h"

class LoginScreen;
class CharacterSelectionScreen;
class LoadingScreen;
class GameScreen;

class Game
{
public:
	Game(const std::string &username, const std::string &password, int character);
	~Game();

	void run();

	void change_to_login_screen();
	void change_to_character_selection_screen();
	void change_to_loading_screen();
	void change_to_game_screen();

	MusicPlayer *get_music_player() { return &music_player; }

private:
	void add_resources_in_directory(clan::ResourceManager &resources, clan::VirtualDirectory &vd, const std::string &directory);

	void on_event_received(const clan::NetGameEvent &e);
	
	std::unique_ptr<UIScreenManager> screen_manager;
	clan::NetGameClient network;
	clan::ResourceManager *resources;

	std::string filename;

	LoginScreen *screen_login;
	CharacterSelectionScreen *screen_character_selection;
	LoadingScreen *screen_loading;
	GameScreen *screen_ingame;

	clan::SoundOutput sound_output;
	MusicPlayer music_player;

	clan::SlotContainer slots;
};