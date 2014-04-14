
#pragma once

#include "Engine/Common/GameWorld/gameobject.h"

enum GameObjectVisual
{
	GOV_CHARACTER = 0,
	GOV_ORC,
	GOV_TROLL,
	GOV_ITEM,
	GOV_SCROLL,
	GOV_POTION,
	GOV_CORPSE
};

class AsciiVisual : public Totem::Component<AsciiVisual>
{
public:
	AsciiVisual(GameObject *owner, const std::string &name = std::string());
	virtual ~AsciiVisual() {}

	static std::string getType() { return "AsciiVisual"; }

protected:
	Totem::Property<int> visual_property;

	Totem::Property<clan::Colorf> color_background_property;
	Totem::Property<clan::Colorf> color_foreground_property;
};
