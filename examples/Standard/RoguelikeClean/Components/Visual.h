#pragma once

#include <Totem/Component.h>
#include <Totem/Property.h>
#include "../Entity.h"
#include "../Math/vec2.h"

#include "ComponentDefinitions.h"

#include <libtcod.hpp>

class Visual : public Totem::Component<Visual>
{
public:
	Visual(Entity *owner);
	virtual ~Visual();

	static std::string GetType() { return COMPONENT_VISUAL; }
	Entity *Owner() const { return _owner; }

public:
	void render(const std::shared_ptr<TCODConsole> &canvas);

private:
	Entity *_owner;

	Totem::Property<int> _layer;
	Totem::Property<int> _symbol;
	Totem::Property<TCODColor> _color;
	Totem::Property<clan::Vec2i> _position;

	void OnCanvasLayerChanged(const int &oldValue, const int &newValue);
};
