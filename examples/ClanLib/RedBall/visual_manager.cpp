
#include "precomp.h"
#include "visual_manager.h"
#include "component_visual.h"

void VisualManager::add_visual(Visual *visual)
{
	visuals.push_back(visual);
}

void VisualManager::render(CL_GraphicContext &gc)
{
	for(size_t i = 0; i < visuals.size(); ++i)
	{
		visuals[i]->render(gc);
	}
}
