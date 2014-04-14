#pragma once

#include "Engine/Common/GameWorld/gameobject.h"

class ServerGameObject : public GameObject
{
// Construction:
public:
	ServerGameObject(int id, const std::string &name, clan::SqliteConnection &db);

	void set_container_id(int container_id);
	void set_persist_changes(bool enable);

	void save_dirty_properties();

	void send_event(const clan::NetGameEvent &event, clan::NetGameConnection *player_connection);

private:
	void on_property_added(std::shared_ptr<Totem::IProperty> property);
	void on_property_removed(std::shared_ptr<Totem::IProperty> property);
	void on_component_added(std::shared_ptr<Totem::IComponent<>> component);
	void on_component_removed(std::shared_ptr<Totem::IComponent<>> component);

	int container_id;
	bool persist_changes;

	clan::SlotContainer slots;
	clan::SqliteConnection &db;
};
