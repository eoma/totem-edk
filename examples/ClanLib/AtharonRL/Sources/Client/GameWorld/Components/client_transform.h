
#pragma once

#include "Engine/Common/GameWorld/Components/transform.h"

class ClientTransform : public Transform
{
public:
	ClientTransform(GameObject *owner, const std::string &name = std::string());
	virtual ~ClientTransform();
};
