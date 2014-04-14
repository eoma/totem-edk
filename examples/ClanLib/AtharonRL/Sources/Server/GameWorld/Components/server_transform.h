
#pragma once

#include "Engine/Common/GameWorld/Components/transform.h"
#include "Server/GameWorld/replicated_component.h"

class ServerTransform : public Transform, public ReplicatedComponent
{
public:
	ServerTransform(GameObject *owner, const std::string &name = std::string());
	virtual ~ServerTransform();
};
