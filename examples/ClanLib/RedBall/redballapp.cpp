
#include "precomp.h"
#include "redballapp.h"
#include "entity.h"
#include "component_visual.h"
#include "component_bounce.h"
#include "component_colorshift.h"
#include "component_sound.h"
#include "component_horizontal_movement.h"
#include "visual_manager.h"
#include "sound_manager.h"

void RedBallApp::run()
{
	quit = false;
	last_time = 0;

	CL_DisplayWindow window("Balls", 800, 600, false, false);
	slots.connect(window.sig_window_close(), this, &RedBallApp::on_window_close);
	slots.connect(window.get_ic().get_keyboard().sig_key_up(), this, &RedBallApp::on_input_up);
	CL_GraphicContext gc = window.get_gc();

	VisualManager visual_manager;
	SoundManager sound_manager;
	sound_manager.add_sound("bounce", "Resources/bounce2.ogg");
	sound_manager.add_sound("music", "Resources/music.ogg");
	sound_manager.play("music");

	Entity background;
	background.addComponent(std::make_shared<Visual>(&background, visual_manager));
	background.addComponent(std::make_shared<ColorShift>(&background));
	background.get<CL_Sprite>("sprite") = CL_Sprite(gc, "Resources/background.png");
	background.get<CL_Origin>("alignment") = origin_top_left;
	background.get<CL_Colorf>("color") = CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f);
	background.get<CL_Colorf>("color_to") = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	background.get<float>("shift_speed") = 40.0f;
	entities.push_back(&background);

	Entity ball1;
	ball1.addComponent(std::make_shared<Visual>(&ball1, visual_manager));
	ball1.addComponent(std::make_shared<Bounce>(&ball1));
	ball1.addComponent(std::make_shared<Sound>(&ball1, sound_manager));
	ball1.addComponent(std::make_shared<HorizontalMovement>(&ball1));
	ball1.get<CL_Sprite>("sprite") = CL_Sprite(gc, "Resources/Ball2.png");
	ball1.get<CL_Colorf>("color") = CL_Colorf(1.0f, 0.2f, 0.2f, 1.0f);
	ball1.get<CL_Pointf>("scale") = CL_Pointf(0.4f, 0.4f);
	ball1.get<CL_Pointf>("position") = CL_Pointf(200.0f, 500.0f);
	ball1.get<CL_Origin>("alignment") = origin_bottom_center;
	entities.push_back(&ball1);

	Entity ball1_shadow;
	ball1_shadow.addComponent(std::make_shared<Visual>(&ball1_shadow, visual_manager));
	ball1_shadow.addComponent(std::make_shared<HorizontalMovement>(&ball1_shadow));
	ball1_shadow.addComponent(std::make_shared<ColorShift>(&ball1_shadow));
	ball1_shadow.get<CL_Sprite>("sprite") = CL_Sprite(gc, "Resources/Shadow.png");
	ball1_shadow.get<CL_Pointf>("scale") = CL_Pointf(0.4f, 0.4f);
	ball1_shadow.get<CL_Pointf>("position") = CL_Pointf(200.0f, 506.0f);
	ball1_shadow.get<CL_Colorf>("color") = CL_Colorf(1.0f, 1.0f, 1.0f, 0.3f);
	ball1_shadow.get<CL_Colorf>("color_to") = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	ball1_shadow.get<float>("shift_speed") = 1000.0f;
	entities.push_back(&ball1_shadow);

	Entity ball2;
	ball2.addComponent(std::make_shared<Visual>(&ball2, visual_manager));
	ball2.addComponent(std::make_shared<Bounce>(&ball2));
	ball2.addComponent(std::make_shared<ColorShift>(&ball2));
	ball2.addComponent(std::make_shared<Sound>(&ball2, sound_manager));
	ball2.get<CL_Sprite>("sprite") = CL_Sprite(gc, "Resources/Ball2.png");
	ball2.get<CL_Colorf>("color") = CL_Colorf(1.0f, 0.2f, 0.2f, 1.0f);
	ball2.get<CL_Colorf>("color_to") = CL_Colorf(0.5f, 0.5f, 0.5f, 1.0f);
	ball2.get<CL_Pointf>("scale") = CL_Pointf(0.6f, 0.6f);
	ball2.get<CL_Pointf>("position") = CL_Pointf(400.0f, 550.0f);
	ball2.get<CL_Origin>("alignment") = origin_bottom_center;
	ball2.get<float>("bounce_height") = 150.0f;
	ball2.get<float>("bounce_speed") = 4.5f;
	entities.push_back(&ball2);

	Entity ball2_shadow;
	ball2_shadow.addComponent(std::make_shared<Visual>(&ball2_shadow, visual_manager));
	ball2_shadow.addComponent(std::make_shared<ColorShift>(&ball2_shadow));
	ball2_shadow.get<CL_Sprite>("sprite") = CL_Sprite(gc, "Resources/Shadow.png");
	ball2_shadow.get<CL_Pointf>("scale") = CL_Pointf(0.6f, 0.6f);
	ball2_shadow.get<CL_Pointf>("position") = CL_Pointf(400.0f, 556.0f);
	ball2_shadow.get<CL_Colorf>("color") = CL_Colorf(1.0f, 1.0f, 1.0f, 0.3f);
	ball2_shadow.get<CL_Colorf>("color_to") = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	ball2_shadow.get<float>("shift_speed") = 900.0f;
	entities.push_back(&ball2_shadow);

	Entity ball3;
	ball3.addComponent(std::make_shared<Visual>(&ball3, visual_manager));
	ball3.addComponent(std::make_shared<ColorShift>(&ball3));
	ball3.get<CL_Sprite>("sprite") = CL_Sprite(gc, "Resources/Ball2.png");
	ball3.get<CL_Colorf>("color_to") = CL_Colorf(0.5f, 0.5f, 0.5f, 0.0f);
	ball3.get<CL_Pointf>("scale") = CL_Pointf(0.5f, 0.5f);
	ball3.get<CL_Pointf>("position") = CL_Pointf(570.0f, 520.0f);
	ball3.get<CL_Origin>("alignment") = origin_bottom_center;
	entities.push_back(&ball3);

	Entity ball3_shadow;
	ball3_shadow.addComponent(std::make_shared<Visual>(&ball3_shadow, visual_manager));
	ball3_shadow.addComponent(std::make_shared<ColorShift>(&ball3_shadow));
	ball3_shadow.get<CL_Sprite>("sprite") = CL_Sprite(gc, "Resources/Shadow.png");
	ball3_shadow.get<CL_Pointf>("scale") = CL_Pointf(0.5f, 0.5f);
	ball3_shadow.get<CL_Pointf>("position") = CL_Pointf(570.0f, 530.0f);
	ball3_shadow.get<CL_Colorf>("color_to") = CL_Colorf(1.0f, 1.0f, 1.0f, 0.0f);
	entities.push_back(&ball3_shadow);

	while (!quit)
	{
		float time_delta_ms = calculate_delta_time();

		for(size_t i = 0; i < entities.size(); ++i)
			entities[i]->update(time_delta_ms);

		visual_manager.render(gc);

		window.flip(1);
		CL_KeepAlive::process(0);
	}
}

float RedBallApp::calculate_delta_time()
{
	if(last_time == 0)
		last_time = CL_System::get_time();

	unsigned int current_time = CL_System::get_time();
	float time_delta_ms = static_cast<float> (current_time - last_time);
	last_time = current_time;

	return time_delta_ms;
}

void RedBallApp::on_input_up(const CL_InputEvent &key)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

void RedBallApp::on_window_close()
{
	quit = true;
}
