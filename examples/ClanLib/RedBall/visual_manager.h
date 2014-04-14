
#pragma once

class Visual;

class VisualManager
{
public:
	void add_visual(Visual *visual);

	void render(CL_GraphicContext &gc);

private:
	std::vector<Visual *> visuals;
};
