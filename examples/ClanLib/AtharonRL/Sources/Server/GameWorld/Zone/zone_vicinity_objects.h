#pragma once

class ServerGameObject;
class Zone;

class ZoneVicinityObjects
{
// Construction:
public:
	ZoneVicinityObjects(clan::NetGameConnection *connection);
	~ZoneVicinityObjects();

// Attributes:
public:
	ServerGameObject *find(int id) const;
	const std::vector<ServerGameObject *> &get_visible_objects() const;

// Operations:
public:
	void sync_dirty_gameobjects();
	void clear_dirty_gameobjects();

	void gameobject_position_changed(ServerGameObject *gameobject);
	void add_gameobject(ServerGameObject *gameobject);
	void remove_gameobject(ServerGameObject *gameobject);

// Implementation:
private:
	void add_visible_object(ServerGameObject *object);
	void remove_visible_object(ServerGameObject *object);
	bool is_object_in_visibility(ServerGameObject *gameobject) const;

	clan::NetGameConnection *connection;

	std::vector<ServerGameObject *> visible_objects;
};
