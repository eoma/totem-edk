
#pragma once

#include "Engine/Common/GameWorld/gameobject.h"

class Transform : public Totem::Component<Transform>
{
public:
	Transform(GameObject *owner, const std::string &name = std::string());
	virtual ~Transform() {}

	static std::string getType() { return "Transform"; }

protected:
	Totem::Property<clan::Vec2i> position_property;
};
