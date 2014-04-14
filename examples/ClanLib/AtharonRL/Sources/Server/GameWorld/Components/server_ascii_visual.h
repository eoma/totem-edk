
#pragma once

#include "Engine/Common/GameWorld/Components/ascii_visual.h"
#include "Server/GameWorld/replicated_component.h"

class ServerAsciiVisual : public AsciiVisual, public ReplicatedComponent
{
public:
	ServerAsciiVisual(GameObject *owner, const std::string &name = std::string());
	virtual ~ServerAsciiVisual();
};
