#include "precomp.h"
#include "layer_manager.h"
#include "layer.h"

LayerManager::LayerManager(const clan::Point &screen_size)
	: screen_size(screen_size)
{
}
LayerManager::~LayerManager()
{
}

void LayerManager::add(const LayerPtr &layer, int zdepth)
{
	if(zdepth == -1)
		layers.push_back(layer);
	else
	{
		for(unsigned int i = 0; i < layers.size(); i++)
		{
			if(layers[i]->get_zdepth() == -1 || zdepth < layers[i]->get_zdepth())
			{
				layer->set_zdepth(zdepth);
				layers.insert(layers.begin()+i, layer);
				return;
			}
		}

		layer->set_zdepth(zdepth);
		layers.push_back(layer);
	}
}
void LayerManager::remove(const LayerPtr &layer)
{
	for(unsigned int i = 0; i < layers.size(); i++)
	{
		if(layers[i].get() == layer.get())
		{
			layers.erase(layers.begin()+i);
			return;
		}
	}
}

LayerPtr LayerManager::get(const std::string &layer_name)
{
	for(unsigned int i = 0; i < layers.size(); i++)
	{
		if(layers[i]->get_layer_name() == layer_name)
			return layers[i];
	}
	return nullptr;
}

void LayerManager::clear()
{
	for(unsigned int i = 0; i < layers.size(); i++)
	{
		layers[i]->clear();
	}
}

void LayerManager::draw(clan::Canvas &canvas, int x, int y)
{
	for(unsigned int i = 0; i < layers.size(); i++)
	{
		layers[i]->draw(canvas, x,y);
	}
}