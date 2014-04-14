
#include "precomp.h"
#include "ui_model_scene.h"

using namespace clan;

UIModelScene::UIModelScene(GUIComponent *parent)
: UIComponent(parent)
{
	init();
}

UIModelScene::UIModelScene(UIScreen *screen)
: UIComponent(screen)
{
	init();
}

void UIModelScene::init()
{
	func_render().set(this, &UIModelScene::on_render);
/*
	GraphicContext gc = get_gc();
	scene_view.reset(new ModelView(gc));
	scene_view->use_skybox_gradient = false;
	scene_view->use_plane = false;
*/
}

void UIModelScene::on_render(Canvas &canvas, const Rect &update_box)
{
/*
	Rect client_box = get_size();
	scene_view->set_viewport(get_size());
	scene_view->render(gc);
*/
}
