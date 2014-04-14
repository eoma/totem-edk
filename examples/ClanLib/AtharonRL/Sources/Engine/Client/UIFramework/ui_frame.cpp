
#include "precomp.h"
#include "ui_frame.h"

using namespace clan;

UIFrame::UIFrame(GUIComponent *parent)
: UIComponent(parent)
{
	func_render().set(this, &UIFrame::on_render);
}

UIFrame::UIFrame(UIScreen *screen)
: UIComponent(screen)
{
	func_render().set(this, &UIFrame::on_render);
}

void UIFrame::on_render(Canvas &canvas, const Rect &update_box)
{
	Rect client_box = get_size();
	background.render(canvas, client_box, get_scale());
}

void UIFrame::update_geometry()
{
	UIComponent::update_geometry();
}