
#include "precomp.h"
#include "character_selection_screen.h"
#include "game.h"
#include "Engine/Client/UIFramework/window_manager.h"
#include "Engine/Client/UIFramework/ui_frame.h"
#include "Engine/Client/UIFramework/ui_button.h"
#include "Engine/Client/UIFramework/ui_lineedit.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

CharacterSelectionScreen::CharacterSelectionScreen(UIScreenManager *screen_manager, Game *game, NetGameClient &network, clan::ResourceManager &resources)
: UIScreen(screen_manager), network(network), game(game), current_character(0)
{
	GraphicContext gc = screen_manager->get_window().get_gc();
	//background = Image(gc, arg+"Resources/Backgrounds/Login.png");
	background = Image(gc, "Background/login", &resources);

	button_create_new = new UIButton(this);
	button_create_new->label.set_text("Create New Character");
	button_create_new->set_scaled_geometry(ScaledBox::top_right_box(50.0f, 50.0f, 255.0f, 25.5f));
	button_create_new->func_clicked().set(this, &CharacterSelectionScreen::on_create_new_clicked);

	for (size_t i = 0; i < 10; i++)
	{
		frame_character.push_back(new UIFrame(this));
		button_character.push_back(new UIButton(frame_character[i]));

		frame_character[i]->set_scaled_geometry(ScaledBox::top_right_box(50.0f, 100.0f + 30.0f * i, 255.0f, 25.5f));
		frame_character[i]->background.outset_left = 4.0f;
		frame_character[i]->background.outset_top = 4.0f;
		frame_character[i]->background.outset_right = 4.0f;
		frame_character[i]->background.outset_bottom = 4.0f;
		frame_character[i]->background.slice_left = 65;
		frame_character[i]->background.slice_right = 58;
		frame_character[i]->background.slice_top = 65;
		frame_character[i]->background.slice_bottom = 65;
		frame_character[i]->background.border_width_left = 4.0f;
		frame_character[i]->background.border_width_right = 4.0f;
		frame_character[i]->background.border_width_top = 4.0f;
		frame_character[i]->background.border_width_bottom = 4.0f;
		if (i == current_character)
		{
			//frame_character[i]->background.image = Image(gc, arg+"Resources/Engine/GUIThemeAero/Images/XP/frame.png");
			frame_character[i]->background.image = Image(gc, "UI/xp_frame", &resources);
		}

		button_character[i]->label.set_text(string_format("Character %1", i));
		button_character[i]->set_scaled_geometry(ScaledBox::top_left_box(0.0f, 0.0f, 255.0f, 25.5f));
		button_character[i]->func_clicked().set(this, &CharacterSelectionScreen::on_select_character, i);
		frame_character[i]->set_visible(false, false);
	}

	button_join = new UIButton(this);
	button_join->label.set_text("Join Game");
	button_join->label.set_font_size(25.0f);
	button_join->set_scaled_geometry(ScaledBox::bottom_center_box(0.0f, 100.0f, 255.0f, 25.5f));
	button_join->func_clicked().set(this, &CharacterSelectionScreen::on_join_clicked);

	label_status = new UILabel(this);
	label_status->set_scaled_geometry(ScaledBox::bottom_center_box(0.0f, 60.0f, 400.0f, 25.5f));
	label_status->set_align(UILabel::text_align_center);

	slots.connect(network.sig_event_received(), this, &CharacterSelectionScreen::on_event_received);

	netevents.func_event(STC_CHARACTER_LIST).set(this, &CharacterSelectionScreen::on_event_character_list);
	netevents.func_event(STC_CHARACTER_LOGIN_SUCCESS).set(this, &CharacterSelectionScreen::on_event_character_login_success);
	netevents.func_event(STC_CHARACTER_LOGIN_FAIL).set(this, &CharacterSelectionScreen::on_event_character_login_fail);
	netevents.func_event(STC_CHARACTER_LOGOUT).set(this, &CharacterSelectionScreen::on_event_character_logout);
	netevents.func_event(STC_CHARACTER_CREATE_SUCCESS).set(this, &CharacterSelectionScreen::on_event_character_create_success);
	netevents.func_event(STC_CHARACTER_CREATE_FAIL).set(this, &CharacterSelectionScreen::on_event_character_create_fail);
}

void CharacterSelectionScreen::on_activated()
{
	UIScreen::on_activated();
	network.send_event(NetGameEvent(CTS_CHARACTER_GETLIST));
}

void CharacterSelectionScreen::update()
{
	Canvas canvas = get_canvas();

	canvas.clear(Colorf::black);
	float scale = canvas.get_height() / (float)background.get_height();
	Size size((int)(background.get_width() * scale), (int)(background.get_height() * scale));
	Rect box(Point((canvas.get_width() - size.width) / 2, 0), size);
	background.draw(canvas, box);

	UIScreen::update();
}

void CharacterSelectionScreen::add_character(int id, const std::string &name)
{
	if (button_character.size() > button_character_ids.size())
	{
		size_t index = button_character_ids.size();
		button_character[index]->label.set_text(name);
		frame_character[index]->set_visible(true, false);
		button_character_ids.push_back(id);
	}
}

void CharacterSelectionScreen::on_event_received(const NetGameEvent &e)
{
	netevents.dispatch(e);
}

void CharacterSelectionScreen::on_event_character_list(const NetGameEvent &e)
{
	unsigned int count = e.get_argument(0);

	for(size_t i = 0; i < count; ++i)
	{
		int id = e.get_argument(1 + i * 2);
		std::string name = e.get_argument(1 + i*2 + 1);

		add_character(id, name);
	}
}

void CharacterSelectionScreen::on_event_character_login_success(const NetGameEvent &e)
{
	//game->change_to_loading_screen();
	game->change_to_game_screen();
}

void CharacterSelectionScreen::on_event_character_login_fail(const NetGameEvent &e)
{
	std::string reason = e.get_argument(0);

	label_status->set_text(reason);

	button_join->set_visible(true);
}

void CharacterSelectionScreen::on_event_character_logout(const NetGameEvent &e)
{
}

void CharacterSelectionScreen::on_event_character_create_success(const NetGameEvent &e)
{
}

void CharacterSelectionScreen::on_event_character_create_fail(const NetGameEvent &e)
{
}

void CharacterSelectionScreen::on_create_new_clicked()
{
	label_status->set_text("Create New Character is not supported yet");
}

void CharacterSelectionScreen::on_select_character(size_t i)
{
	if (i != current_character)
	{
		frame_character[i]->background.image = frame_character[current_character]->background.image;
		frame_character[current_character]->background.image = Image();
		current_character = i;
	}
}

void CharacterSelectionScreen::on_join_clicked()
{
	if(current_character < button_character_ids.size())
	{
		network.send_event(NetGameEvent(CTS_CHARACTER_LOGIN, button_character_ids[current_character]));

		button_join->set_visible(false);

		label_status->set_text("Joining game...");
	}
}
