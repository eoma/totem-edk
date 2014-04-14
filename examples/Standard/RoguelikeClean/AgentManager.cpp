#include "AgentManager.h"
#include "Agent.h"

#include <algorithm>

AgentManager::AgentManager()
{
}

AgentManager::~AgentManager()
{
}

void AgentManager::add(std::shared_ptr<Agent> agent)
{
	auto it = std::find(_agents.begin(), _agents.end(), agent);
	if(it == _agents.end())
		_agents.push_back(agent);
}

bool AgentManager::remove(std::shared_ptr<Agent> agent)
{
	auto it = std::find(_agents.begin(), _agents.end(), agent);
	if(it != _agents.end())
	{
		_agents.erase(it);
		return true;
	}

	return false;
}

void AgentManager::think(int elapsedTime)
{
	for(auto agent : _agents)
	{
		agent->think(elapsedTime);
	}
}
