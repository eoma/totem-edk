#pragma once

#include <libtcod.hpp>
#include <memory>
#include <vector>

class Visual;

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	enum CanvasLayer
	{
		LAYER_GROUND,
		LAYER_CLUTTER,
		LAYER_ITEM,
		LAYER_CORPSE,
		LAYER_ACTOR,
		LAYER_AIR,
		CANVAS_LAYER_COUNT
	};

	void initialize();
	void render();

	static CanvasLayer toLayer(int layer) 
	{ 
		if(layer < 0 || layer >= CANVAS_LAYER_COUNT) 
			return CANVAS_LAYER_COUNT; 
		else 
			return (CanvasLayer)layer; 
	}

	void add(Visual *visual, CanvasLayer layer);
	bool remove(Visual *visual, CanvasLayer layer);

private:
	class Canvas
	{
	public:
		Canvas(CanvasLayer layer) : layer(layer), backgroundAlpha(0.0f) {}
		~Canvas() {}

		const CanvasLayer layer;
		std::shared_ptr<TCODConsole> console;
		std::vector<Visual*> visuals;
		float backgroundAlpha;
	};
	std::vector<std::shared_ptr<Canvas>> _canvasLayers;

	std::shared_ptr<Canvas> _create(CanvasLayer layer, float backgroundAlpha = 0.0f);
	std::shared_ptr<Canvas> _find(CanvasLayer layer);

	static bool SortCanvas(const std::shared_ptr<Canvas> &a, const std::shared_ptr<Canvas> &b)
	{
		if(a == nullptr)
			return false;
		if(b == nullptr)
			return true;
		return a->layer < b->layer;
	}
};
