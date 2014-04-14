#include "Ai.h"
#include "../GameManager.h"
#include "../AgentManager.h"
#include "../Agent.h"
#include "../PropertyDefinitions.h"

using namespace clan;
using namespace Totem;

Ai::Ai(Entity *owner)
	: Component<Ai>(GetType()), _owner(owner)
{
	_agent = std::make_shared<Agent>();
	_agent->Set(owner->SafePtr());
	GameManager::Get()->getAgent()->add(_agent);
}

Ai::~Ai() 
{
	GameManager::Get()->getAgent()->remove(_agent);
	_agent.reset();
}
