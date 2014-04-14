#include "RenderManager.h"
#include "Components/Visual.h"
#include "GameManager.h"
#include "MapManager.h"

#include <algorithm>

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::initialize()
{
	_canvasLayers.resize(CANVAS_LAYER_COUNT, nullptr);
	_create(LAYER_GROUND, 1.0f);
	_create(LAYER_AIR, 0.25f);
}

void RenderManager::render()
{
	for(auto layer : _canvasLayers)
	{
		if(layer == nullptr)
			continue;

		bool blitLayer = false;

		if(layer->layer == LAYER_GROUND)
		{
			GameManager::Get()->getMap()->render(layer->console, MapManager::LAYER_GROUND);
			blitLayer = true;
		}
		else if(layer->layer == LAYER_AIR)
		{
			GameManager::Get()->getMap()->render(layer->console, MapManager::LAYER_AIR);
			blitLayer = true;
		}

		for(auto visual : layer->visuals)
		{
			visual->render(layer->console);
			blitLayer = true;
		}
		if(blitLayer)
		{
			TCODConsole::blit(layer->console.get(), 0,0, layer->console->getWidth(), layer->console->getHeight(), TCODConsole::root, 0,0, 1.0f, layer->backgroundAlpha);
			layer->console->clear();
		}
	}
}

void RenderManager::add(Visual *visual, CanvasLayer layer)
{
	auto canvasLayer = _find(layer);
	if(canvasLayer == nullptr)
	{
		canvasLayer = _create(layer);
		//sort( _canvasLayers.begin(), _canvasLayers.end(), RenderManager::SortCanvas );
	}
	
	canvasLayer->visuals.push_back(visual);
}

bool RenderManager::remove(Visual *visual, CanvasLayer layer)
{
	auto canvasLayer = _find(layer);
	if(canvasLayer)
	{
		for(auto it = canvasLayer->visuals.begin(); it != canvasLayer->visuals.end(); ++it)
		{
			if((*it) == visual)
			{
				canvasLayer->visuals.erase(it);
				/*if(canvasLayer->visuals.empty() && (canvasLayer->layer != LAYER_GROUND || canvasLayer->layer != LAYER_AIR))
				{
					for(auto it2 = _canvasLayers.begin(); it2 != _canvasLayers.end(); ++it2)
					{
						if((*it2) == canvasLayer)
						{
							_canvasLayers.erase(it2);
							//sort( _canvasLayers.begin(), _canvasLayers.end(), RenderManager::SortCanvas );
							return true;
						}
					}
				}*/
				return true;
			}
		}
	}

	return false;
}

std::shared_ptr<RenderManager::Canvas> RenderManager::_create(CanvasLayer layer, float backgroundAlpha)
{
	auto canvasLayer = std::make_shared<Canvas>(layer);
	canvasLayer->console = std::make_shared<TCODConsole>(TCODConsole::root->getWidth(), TCODConsole::root->getHeight());
	canvasLayer->backgroundAlpha = backgroundAlpha;
	//if(layer != LAYER_GROUND)
	//	canvasLayer->console->setKeyColor(TCODColor::black);

	_canvasLayers[layer] = canvasLayer;
	return canvasLayer;
}

std::shared_ptr<RenderManager::Canvas> RenderManager::_find(CanvasLayer layer)
{
	return _canvasLayers[layer];
	/*for(auto canvasLayer : _canvasLayers)
	{
		if(canvasLayer && canvasLayer->layer == layer)
			return canvasLayer;
	}

	return nullptr;*/
}
