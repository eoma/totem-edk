
#include "precomp.h"
#include "game.h"
#include "Engine/Client/UIFramework/window_manager.h"
#include "LoginScreen/login_screen.h"
#include "CharacterSelectionScreen/character_selection_screen.h"
#include "LoadingScreen/loading_screen.h"
#include "GameScreen/game_screen.h"

using namespace clan;

Game::Game(const std::string &username, const std::string &password, int character)
: screen_login(nullptr), screen_character_selection(nullptr), screen_loading(nullptr), screen_ingame(nullptr)
{
	clan::VirtualFileSystem vfs(".");
	clan::VirtualDirectory vd(vfs, "Resources");

	resources = new clan::ResourceManager(vd.open_file("resources.xml"), vd);
	
	add_resources_in_directory(*resources, vd, "Fonts");
	add_resources_in_directory(*resources, vd, "Engine");

	ScreenInfo screen_info;
	int primary_screen_index = 0;
	std::vector<Rect> screen_boxes = screen_info.get_screen_geometries(primary_screen_index);

	OpenGLWindowDescription desc;
	desc.set_title("Atharon, the Roguelike Client");
//	desc.set_size(Size(screen_boxes[primary_screen_index].get_width() * 90 / 100, (screen_boxes[primary_screen_index].get_height() - 50) * 90 / 100), true);
	desc.set_size(Size(400,400), true);
	desc.set_allow_resize(true);

	screen_manager.reset(new UIScreenManager(desc, "Resources/Icons/gameide-48.png", "Resources/Icons/gameide-16.png"));
//	screen_manager->hide_cursor();
//	screen_manager->maximize();

	sound_output = SoundOutput(44100);
	screen_login = new LoginScreen(screen_manager.get(), this, network, *resources, username, password, character);
	screen_character_selection = new CharacterSelectionScreen(screen_manager.get(), this, network, *resources);
	screen_loading = new LoadingScreen(screen_manager.get(), this, network, *resources);

	slots.connect(network.sig_event_received(), this, &Game::on_event_received);
}

Game::~Game()
{
	delete screen_login;
	delete screen_character_selection;
	delete screen_loading;
	if(screen_ingame)
		delete screen_ingame;
}

void Game::add_resources_in_directory(clan::ResourceManager &resources, clan::VirtualDirectory &vd, const std::string &directory)
{
	auto dir = vd.open_directory(directory);
	auto dir_list = dir.get_directory_listing();
	while(!dir_list.is_null())
	{
		auto file = dir_list.get_filename();
		if(file.find(".xml") == std::string::npos)
		{
			if( dir_list.next() == false )
				break;
			else
				continue;
		}

		resources.add_resources(clan::ResourceManager(dir.open_file(file), dir));

		if( dir_list.next() == false )
			break;
	}
}

void Game::run()
{
	DisplayWindow window = screen_manager->get_window();

	window.show();

	screen_login->connect();
	screen_login->login_hack();

//	change_to_login_screen();
	change_to_game_screen();

	Canvas canvas(window);
	
	bool exit = false;
	Slot slot = window.sig_window_close().connect_functor([&exit] { exit = true; });

	InputContext ic = screen_manager->get_window().get_ic();
	while (!exit)
	{
		if(ic.get_keyboard().get_keycode(keycode_escape))
			exit = true;

		music_player.update();
//		screen_manager->update();

		screen_ingame->update();
		screen_ingame->draw(canvas);
		
		canvas.flip(1);

		KeepAlive::process();
	}

	screen_manager->get_window().hide();
}

void Game::change_to_login_screen()
{
	screen_login->set_active();
}

void Game::change_to_character_selection_screen()
{
	screen_character_selection->set_active();
}

void Game::change_to_loading_screen()
{
	screen_loading->set_active();
}

void Game::change_to_game_screen()
{
	if (screen_ingame == nullptr)
		screen_ingame = new GameScreen(screen_manager.get(), this, network, *resources);
	
//	screen_ingame->set_active();
}

void Game::on_event_received(const NetGameEvent &e)
{
	cl_log_event("Network", "Event: %1", e.to_string());
}
