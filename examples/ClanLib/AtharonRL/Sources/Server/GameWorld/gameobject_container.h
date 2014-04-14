#pragma once

#include "component_factory.h"

class ServerGameObject;

class GameObjectContainer
{
// Construction:
public:
	GameObjectContainer(clan::SqliteConnection &db, int container_id);
	~GameObjectContainer();

// Attributes:
public: 
	const std::vector<ServerGameObject *> &get_all() const { return gameobjects; }

	ServerGameObject *find(const std::string &name) const;
	ServerGameObject *find(int id) const;

	int get_container_id() const { return container_id; }
	unsigned int get_size() const { return gameobjects.size(); }
	bool is_empty() const { return gameobjects.empty(); }

// Operations:
public:
	void load_from_database(std::shared_ptr<ComponentFactory> component_factory);
	ServerGameObject *load_gameobject_from_database(int gameobject_id, std::shared_ptr<ComponentFactory> component_factory);

	void update(float time_elapsed);

	void save_dirty_properties();

	// Returns true is gameobject was added, false otherwise (already exists in container, or is null object)
	bool add(ServerGameObject *gameobject);

	// Removes the gameobject and sets its container to 0.
	// Returns true is gameobject was removed, false otherwise (does not exists in container)
	bool remove(ServerGameObject *gameobject);

	// Removes the gameobject and sets it inactive.
	// Returns true is gameobject was removed, false otherwise (does not exists in container)
	bool set_inactive(ServerGameObject *gameobject);

// Implementation:
protected:
	std::vector<ServerGameObject *> gameobjects;

	int container_id;

	clan::SqliteConnection &db;
};
typedef std::shared_ptr<GameObjectContainer> GameObjectContainerPtr;
