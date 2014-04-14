#pragma once

#include <memory>
#include <vector>

class Agent;

class AgentManager
{
public:
	AgentManager();
	~AgentManager();

	void add(std::shared_ptr<Agent> agent);
	bool remove(std::shared_ptr<Agent> agent);

	void think(int elapsedTime);

private:
	std::vector<std::shared_ptr<Agent>> _agents;
};