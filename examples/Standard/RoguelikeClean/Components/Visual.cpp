#include "Visual.h"
#include "../GameManager.h"
#include "../MapManager.h"
#include "../RenderManager.h"
#include "../Player.h"
#include "../PropertyDefinitions.h"

using namespace clan;
using namespace Totem;

Visual::Visual(Entity *owner)
	: Component<Visual>(GetType()), _owner(owner)
{
	_layer = owner->add<int>(PROPERTY_CANVAS_LAYER, RenderManager::LAYER_GROUND);
	_symbol = owner->add<int>(PROPERTY_SYMBOL, '@');
	_color = owner->add<TCODColor>(PROPERTY_FOREGROUND_COLOR, TCODColor::white);
	_position = owner->add<Vec2i>(PROPERTY_POSITION, Vec2i());

	_layer.valueChanged().connect(this, &Visual::OnCanvasLayerChanged);

	GameManager::Get()->getRender()->add(this, RenderManager::toLayer(_layer));
}

Visual::~Visual() 
{
	GameManager::Get()->getRender()->remove(this, RenderManager::toLayer(_layer));
}

void Visual::render(const std::shared_ptr<TCODConsole> &canvas)
{
	bool render = false;
	bool isPlayer = false;
	if(_owner == GameManager::Get()->getPlayer()->Get().get())
	{
		isPlayer = true;
		render = true;
	}
	else
	{
		if(_layer.get() != RenderManager::LAYER_AIR)
		{
			if(GameManager::Get()->getMap()->isInFov(MapManager::LAYER_GROUND, _position.get()))
				render = true;
		}
		else
		{
			if(GameManager::Get()->getMap()->isInFov(MapManager::LAYER_AIR, _position.get()))
				render = true;
		}
	}

	if(render)
	{
		if(isPlayer)
		{
			canvas->setChar(_position.get().x, _position.get().y, _symbol.get());
			canvas->setCharForeground(_position.get().x, _position.get().y, _color.get());
		}
		else
		{
			auto player = GameManager::Get()->getPlayer()->Get();
			auto position = player->get<Vec2i>(PROPERTY_POSITION).get();
			auto radius = player->get<int>(PROPERTY_SIGHT_RADIUS).get();

			auto distance = position.distance(_position.get());
			auto fadeMod = min(1.0f - (distance / (float)radius) + 0.5f - ((rand()%2+1)*0.025f), 1.0f);
			fadeMod *= fadeMod;

			canvas->setChar(_position.get().x, _position.get().y, _symbol.get());
			canvas->setCharForeground(_position.get().x, _position.get().y, _color.get() * fadeMod);
		}
	}
}

void Visual::OnCanvasLayerChanged(const int &oldValue, const int &newValue)
{
	GameManager::Get()->getRender()->remove(this, RenderManager::toLayer(oldValue));
	GameManager::Get()->getRender()->add(this, RenderManager::toLayer(newValue));
}
