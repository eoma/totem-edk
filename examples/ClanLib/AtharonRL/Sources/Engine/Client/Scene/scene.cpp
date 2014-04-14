
#include "precomp.h"
#include "scene.h"

using namespace clan;

int Scene::instances_drawn = 0;
int Scene::models_drawn = 0;
int Scene::draw_calls = 0;
int Scene::scene_visits = 0;

Scene::Scene()
: frame(0)
{
}

void Scene::render()
{
	ScopeTimeFunction();
}

void Scene::update(float time_elapsed)
{
	// To do: update scene object animations here too
}

