#pragma once

#include <memory>
#include <vector>

class Entity;

class EntityContainer
{
public:
	EntityContainer();
	~EntityContainer();

	const std::vector<std::shared_ptr<Entity>> &getAll() const { return _entities; }

	std::shared_ptr<Entity> find(const std::string &name) const;

	unsigned int getSize() const { return _entities.size(); }
	bool isEmpty() const { return _entities.empty(); }
	void clear() { _entities.clear(); }

	void think(int elapsedTime);

public:
	void update();

	bool add(const std::shared_ptr<Entity> &entity);
	bool remove(const std::shared_ptr<Entity> &entity, bool immediate = false);

private:
	std::vector<std::shared_ptr<Entity>> _entities;
	std::vector<std::shared_ptr<Entity>> _pendingDelete;
};
