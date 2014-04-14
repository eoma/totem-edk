
#include "precomp.h"
#include "login_screen.h"
#include "game.h"
#include "Engine/Client/UIFramework/window_manager.h"
#include "Engine/Client/UIFramework/ui_button.h"
#include "Engine/Client/UIFramework/ui_lineedit.h"
#include "Engine/Client/UIFramework/ui_model_scene.h"
#include "Engine/Common/Network/netevents.h"

using namespace clan;

#define HACK_LOGIN

LoginScreen::LoginScreen(UIScreenManager *screen_manager, Game *game, NetGameClient &network, clan::ResourceManager &resources, const std::string &username, const std::string &password, int character)
: UIScreen(screen_manager), network(network), game(game), username(username), password(password), character(character)
{
	game->get_music_player()->play("Music/main_theme", resources);

	GraphicContext gc = screen_manager->get_window().get_gc();

	//background = Image(gc, arg+"Resources/Backgrounds/Login.png");
	background = Image(gc, "Background/login", &resources);
	background.set_linear_filter(false);

#define STRING2(x) #x
#define STRING(x) STRING2(x)
#pragma message(__FILE__ "(" STRING(__LINE__) "): Font_System::register_font needs a fix")
	//Font_System::register_font("Zombie/Fonts/Univers LT Std 49 Light Ultra Condensed.ttf", "Univers LT Std");

	ScaledSprite sprite_button;
	//sprite_button.image = Image(gc, arg+"Resources/Engine/GUIThemeAero/Images/button_normal.png");
	sprite_button.image = Image(gc, "UI/button_normal", &resources);
	sprite_button.set_border_width(12.0f, 12.0f, 12.0f, 12.0f);
	sprite_button.set_slice(12, 12, 12, 12);

	ScaledSprite sprite_lineedit;
	//sprite_lineedit.image = Image(gc, arg+"Resources/Engine/GUIThemeAero/Images/lineedit_normal.png");
	sprite_lineedit.image = Image(gc, "UI/lineedit_normal", &resources);
	sprite_lineedit.set_border_width(12.0f, 12.0f, 12.0f, 12.0f);
	sprite_lineedit.set_slice(12, 12, 12, 12);

	button_connect = new UIButton(this);
	button_connect->background = sprite_button;
	button_connect->set_scaled_geometry(ScaledBox::bottom_center_box(0.0f, 110.0f, 100.0f, 37.0f));
	button_connect->func_clicked().set(this, &LoginScreen::on_connect_clicked);
	button_connect->set_visible(false);

	UILabel *label_button_connect = new UILabel(button_connect);
	label_button_connect->set_text("Connect");
	label_button_connect->set_font("Univers LT Std", 18.0f);
	label_button_connect->set_drop_shadow(false);
	label_button_connect->set_color(Colorf::white);
	label_button_connect->set_align(UILabel::text_align_center);
	label_button_connect->set_scaled_geometry(ScaledBox::center_box(0.0f, 6.0f, 110.0f, 37.0f));

	frame_login = new UIFrame(this);
	frame_login->set_scaled_geometry(ScaledBox::bottom_center_box(0.0f, 110.0f, 386.0f, 37.0f));
	frame_login->set_visible(false);

	UIFrame *frame_username = new UIFrame(frame_login);
	frame_username->background = sprite_lineedit;
	frame_username->set_scaled_geometry(ScaledBox::center_left_box(0.0f, 0.0f, 150.0f, 37.0f));

	input_username = new UILineEdit(frame_username);
	input_username->set_font("Univers LT Std", 20.0f);
	input_username->set_drop_shadow(false);
	input_username->set_color(Colorf::white);
	input_username->set_placeholder_text("USERNAME");
	input_username->set_placeholder_color(Colorf("#505050"));
	input_username->set_scaled_geometry(ScaledBox::top_left_box(12.0f, 8.0f, 150.0f-24.0f, 37.0f-24.0f));

	UIFrame *frame_password = new UIFrame(frame_login);
	frame_password->background = sprite_lineedit;
	frame_password->set_scaled_geometry(ScaledBox::center_left_box(160.0f, 0.0f, 150.0f, 37.0f));

	input_password = new UILineEdit(frame_password);
	input_password->set_font("Univers LT Std", 20.0f);
	input_password->set_drop_shadow(false);
	input_password->set_color(Colorf::white);
	input_password->set_placeholder_text("PASSWORD");
	input_password->set_placeholder_color(Colorf("#505050"));
	input_password->set_scaled_geometry(ScaledBox::top_left_box(12.0f, 8.0f, 150.0f-24.0f, 37.0f-24.0f));
	input_password->set_password_mode(true);

	button_login = new UIButton(frame_login);
	button_login->background = sprite_button;
	button_login->set_scaled_geometry(ScaledBox::center_left_box(320.0f, 0, 65.0f, 37.0f));
	button_login->func_clicked().set(this, &LoginScreen::on_login_clicked);

	UILabel *label_button_login = new UILabel(button_login);
	label_button_login->set_text("LOG IN");
	label_button_login->set_font("Univers LT Std", 18.0f);
	label_button_login->set_drop_shadow(false);
	label_button_login->set_color(Colorf::white);
	label_button_login->set_align(UILabel::text_align_center);
	label_button_login->set_scaled_geometry(ScaledBox::top_left_box(12.0f, 9.0f, 65.0f-24.0f, 37.0f-24.0f));

	label_login_status = new UILabel(this);
	label_login_status->set_font("Univers LT Std", 18.0f);
	label_login_status->set_color(Colorf::white);
	label_login_status->set_align(UILabel::text_align_center);
	label_login_status->set_scaled_geometry(ScaledBox::bottom_center_box(0.0f, 50.0f, 400.0f, 37.0f));

	css_text =
		"body { color: #cfcfcf; font: 12px Tahoma; margin: 0; }"
		"h1 { font-size: 12px; font-weight: bold; color: #d5ab5b;  margin: 0; }"
		".time { color: #a2a2a2; margin-bottom: 10px;}"
		"ul { margin-left: 12px; }"
		"li { color: #d5ab5b; }";

	slots.connect(network.sig_connected(), this, &LoginScreen::on_connected);
	slots.connect(network.sig_disconnected(), this, &LoginScreen::on_disconnected);
	slots.connect(network.sig_event_received(), this, &LoginScreen::on_event_received);

	netevents.func_event(STC_MOTD).set(this, &LoginScreen::on_event_motd);
	netevents.func_event(STC_LOGIN_FAIL).set(this, &LoginScreen::on_event_login_fail);
	netevents.func_event(STC_LOGIN_SUCCESS).set(this, &LoginScreen::on_event_login_success);

	UIModelScene *model_scene = new UIModelScene(this);
	model_scene->set_scaled_geometry(ScaledBox::top_right_box(50.0f, 100.0f, 400.0f, 300.0f));
/*
	ModelInstance textured_plane(model_scene->scene_view->get_model(gc, "Zombie/Models/Misc/TexturedPlane/TexturedPlane.cmodel"));
	textured_plane.set_position(Vec3f(0.0f, -1.0f, 0.0f));
	textured_plane.set_scale(Vec3f(10.0f));
	textured_plane.set_orientation(Quaternionf(0.0f, 90.0f, 90.0f, angle_degrees, order_ZYX));
	model_scene->scene_view->models.push_back(textured_plane);

	ModelInstance instance(model_scene->scene_view->get_model(gc, "Zombie/Models/Weapons/Shotgun/shotgun.cmodel"));
	instance.set_position(Vec3f(-0.8f, 0.0f, 0.0f));
	instance.set_scale(Vec3f(0.01f));
	instance.set_orientation(Quaternionf(0.0f, 90.0f, 90.0f, angle_degrees, order_ZYX));
	model_scene->scene_view->models.push_back(instance);

	float up_angle = 30.0f;
	float dir_angle = -40.0f;
	model_scene->scene_view->zoom = 2.0f;
	model_scene->scene_view->camera_dir = Quaternionf(up_angle, dir_angle, 0.0f, angle_degrees, order_XYZ);
*/
#ifdef _DEBUG
	input_username->set_text("sphair");
	input_password->set_text("p");
#endif
}

void LoginScreen::on_activated()
{
	UIScreen::on_activated();
	connect();
}

void LoginScreen::update()
{
	Canvas canvas = get_canvas();

	canvas.clear(Colorf::black);
	float scale = canvas.get_height() / (float)background.get_height();
	Size size((int)(background.get_width() * scale), (int)(background.get_height() * scale));
	Rect box(Point((canvas.get_width() - size.width) / 2, 0), size);
	background.draw(canvas, box);

	UIScreen::update();
}

void LoginScreen::connect()
{
	std::string html =
		"<html>"
		"<body>"
		"<div>Connecting...</div>"
		"</body>"
		"</html>";

#ifdef _DEBUG
	std::string server = "localhost";
#else
	std::string server = "billowe.no-ip.org";
#endif
	std::string port = "4556";

	network.connect(server, port);
}

void LoginScreen::on_connect_clicked()
{
	connect();
}

void LoginScreen::login_hack()
{
	network.send_event(NetGameEvent(CTS_LOGIN, username, password));
}

void LoginScreen::on_login_clicked()
{
	label_login_status->set_text("");

	std::string username = input_username->get_text();
	std::string password = input_password->get_text();

	network.send_event(NetGameEvent(CTS_LOGIN, username, password));
}

void LoginScreen::on_connected()
{
#ifdef HACK_LOGIN
	return;
#endif
	cl_log_event("Network", "Connected");

	frame_login->set_visible(true);
	button_connect->set_visible(false);
}

void LoginScreen::on_disconnected()
{
#ifdef HACK_LOGIN
	return;
#endif
	cl_log_event("Network", "Disconnected");

	std::string html =
		"<html>"
		"<body>"
		"<h1>Server down</h1>"
		"<div>Unable to connect to server. Try again later.</div>"
		"</body>"
		"</html>";

	frame_login->set_visible(false);
	button_connect->set_visible(true);
}

void LoginScreen::on_event_received(const NetGameEvent &e)
{
	netevents.dispatch(e);
}

void LoginScreen::on_event_login_fail(const NetGameEvent &e)
{
#ifdef HACK_LOGIN
	return;
#endif
	std::string reason = e.get_argument(0);

	label_login_status->set_text(reason);
}

void LoginScreen::on_event_login_success(const NetGameEvent &e)
{
#ifdef HACK_LOGIN
	network.send_event(NetGameEvent(CTS_CHARACTER_LOGIN, character));
	game->change_to_game_screen();
#else
	game->change_to_character_selection_screen();
#endif
}

void LoginScreen::on_event_motd(const NetGameEvent &e)
{
#ifdef HACK_LOGIN
	return;
#endif
	std::string html = e.get_argument(0);
}
