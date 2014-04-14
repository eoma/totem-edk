
#include "precomp.h"
#include "loading_screen.h"
#include "game.h"
#include "Engine/Client/UIFramework/window_manager.h"
#include "Engine/Client/UIFramework/ui_label.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

LoadingScreen::LoadingScreen(UIScreenManager *screen_manager, Game *game, NetGameClient &network, clan::ResourceManager &resources)
: UIScreen(screen_manager), game(game), network(network)
{
	GraphicContext gc = screen_manager->get_window().get_gc();
	background = Image(gc, "Background/login", &resources);

	label = new UILabel(this);
	label->set_scaled_geometry(ScaledBox::center_box(0.0f, 0.0f, 127.5f, 25.0f));
	label->set_text("Loading...");

	hack_counter = 0;
}

void LoadingScreen::on_activated()
{
	UIScreen::on_activated();
}

void LoadingScreen::update()
{
	Canvas canvas = get_canvas();

	canvas.clear(Colorf::black);
	float scale = canvas.get_height() / (float)background.get_height();
	Size size((int)(background.get_width() * scale), (int)(background.get_height() * scale));
	Rect box(Point((canvas.get_width() - size.width) / 2, 0), size);
	background.draw(canvas, box);

	UIScreen::update();

	// Make sure we draw this screen before we switch to game
	if(hack_counter++ > 100)
		game->change_to_game_screen();
}
