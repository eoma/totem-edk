
#include "precomp.h"
#include "game_screen.h"
#include "game.h"
#include "Engine/Client/UIFramework/window_manager.h"
#include "Engine/Common/Network/netevents.h"

#include "Engine/Client/Scene/layer.h"

using namespace clan;

GameScreen::GameScreen(UIScreenManager *screen_manager, Game *game, NetGameClient &network, clan::ResourceManager &resources)
: /*UIScreen(screen_manager), */network(network), game(game)
{
	slots.connect(network.sig_event_received(), this, &GameScreen::on_event_received);

	GraphicContext gc = screen_manager->get_window().get_gc();
	
	auto glyph_size = Point(16,29);
	auto layer_screen_size = Point((gc.get_width() / glyph_size.x), (gc.get_height() / glyph_size.y));
	//auto layer_screen_size = Point(3,2);
	layer_manager.reset(new LayerManager(layer_screen_size));

	{
		auto tile_data = std::make_shared<TileData>(Colorf::black, Colorf::white, ' ');
		auto description = LayerDescription(Point(0,0), layer_manager->get_screen_size(), tile_data);
		auto bitmap = LayerBitmap(Sprite(gc, "Font/font-10", &resources), glyph_size, nullptr);
		dungeon_layer = std::shared_ptr<Layer>(new Layer("Dungeon", description, bitmap));
		layer_manager->add(dungeon_layer);
	}
	{
		auto tile_data = std::make_shared<TileData>(Colorf::black, Colorf::white, ' ');
		auto description = LayerDescription(Point(0,0), layer_manager->get_screen_size(), tile_data);
		auto bitmap = LayerBitmap(Sprite(gc, "Font/font-10", &resources), glyph_size, nullptr);
		character_layer = std::shared_ptr<Layer>(new Layer("Character", description, bitmap));
		layer_manager->add(character_layer);
	}

	zone.reset(new ClientZone(network, layer_manager));

	InputContext ic = screen_manager->get_window().get_ic();
	slots.connect(ic.get_keyboard().sig_key_down(), this, &GameScreen::on_key_down);

	is_awaiting_response = false;
}

void GameScreen::on_activated()
{
//	UIScreen::on_activated();
}

void GameScreen::on_key_down(const InputEvent &event)
{
	if(event.id == keycode_numpad1) move(-1,1);
	else if(event.id == keycode_numpad2) move(0,1);
	else if(event.id == keycode_numpad3) move(1,1);
	else if(event.id == keycode_numpad4) move(-1,0);
	else if(event.id == keycode_numpad5) move(0,0);
	else if(event.id == keycode_numpad6) move(1,0);
	else if(event.id == keycode_numpad7) move(-1,-1);
	else if(event.id == keycode_numpad8) move(0,-1);
	else if(event.id == keycode_numpad9) move(1,-1);
}

void GameScreen::move(int delta_x, int delta_y)
{
	is_awaiting_response = true;
	network.send_event(NetGameEvent(CTS_PLAYER_MOVE, delta_x, delta_y));
}

void GameScreen::update()
{
	//Clear layers before we let zone fill it with new data
	layer_manager->clear();

	zone->tick(0.0167f);

//	UIScreen::update();
}

void GameScreen::draw(Canvas &canvas)
{
//	Canvas canvas = get_canvas();

	canvas.clear();
	layer_manager->draw(canvas, 0,0);
}

void GameScreen::on_event_received(const NetGameEvent &e)
{
	zone->dispatch_net_event(e);
}
