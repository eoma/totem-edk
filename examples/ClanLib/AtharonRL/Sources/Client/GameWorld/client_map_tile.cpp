#include "precomp.h"
#include "client_map_tile.h"
#include "client_zone.h"
#include "client_camera.h"
#include "Engine/Client/Scene/layer_manager.h"
#include "Engine/Client/Scene/layer.h"

ClientMapTile::ClientMapTile(const clan::Vec2i &position, bool walkable, ClientZone *zone, const LayerManagerPtr &layer_manager)
	: MapTileInterface(position), walkable(walkable), layer_manager(layer_manager)
{
	dungeon_layer = layer_manager->get("Character");
}

ClientMapTile::~ClientMapTile()
{
}

void ClientMapTile::update(float time_elapsed)
{
}

void ClientMapTile::draw(const ClientCameraPtr &camera)
{
	int character = (walkable ? '.' : '#');

	auto view_space_position = position - camera->get_view() + camera->get_projection();
	auto tile = std::make_shared<TileData>(clan::Colorf::black, clan::Colorf::white, character);
	dungeon_layer->set_tile(view_space_position, tile);
}
