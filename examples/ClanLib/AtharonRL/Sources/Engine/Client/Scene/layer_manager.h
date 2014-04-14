#pragma once

#include <memory> 

class Layer; typedef std::shared_ptr<Layer> LayerPtr;

class LayerManager
{
public:
	LayerManager(const clan::Point &screen_size);
	~LayerManager();

	const clan::Point &get_screen_size() const { return screen_size; }

	void add(const LayerPtr &layer, int zdepth = -1); //If zdepth is -1, it's pushed to the back of the layer list
	void remove(const LayerPtr &layer);
	LayerPtr get(const std::string &layer_name);
	const std::vector<LayerPtr> &getLayers() const { return layers; }

	void clear();
	void draw(clan::Canvas &canvas, int x, int y);

private:
	clan::Point screen_size;
	std::vector<LayerPtr> layers;
};