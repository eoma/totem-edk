
#pragma once

#include <list>

class Scene
{
public:
	Scene();

	void render();
	void update(float time_elapsed);

	static int models_drawn;
	static int instances_drawn;
	static int draw_calls;
	static int scene_visits;

private:
	int frame;
};
