#pragma once

#include "Entity.h"

#include "Math/vec2.h"
#include <memory>
#include <vector>

class Controller : public std::enable_shared_from_this<Controller>
{
public:
	Controller();
	virtual ~Controller();

	void think(int elapsedTime = 0);

	void Set(const std::shared_ptr<Entity> &pawn);
	std::shared_ptr<Entity> Get() const { if(_minions[0]) return _minions[0]->entity; else return nullptr; }

	void Add(const std::shared_ptr<Entity> &pawn);
	const std::vector<std::shared_ptr<Entity>> &GetAll() const { return _allEntities; }

	virtual int estimateAction(unsigned int index = 0);
	virtual void takeAction(unsigned int index = 0);

protected:
	virtual void _internalThink(int elapsedTime) = 0;
	void _resetActionIntent();

protected:
	class Minion
	{
	public:
		std::shared_ptr<Entity> entity;
		clan::Vec2i dir;
		std::vector<bool> actionIntent;
		int currentIntent;
	};
	std::vector<std::shared_ptr<Minion>> _minions;
	std::vector<std::shared_ptr<Entity>> _allEntities;
};