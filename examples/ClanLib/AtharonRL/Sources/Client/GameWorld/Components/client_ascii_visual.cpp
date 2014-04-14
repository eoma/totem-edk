
#include "precomp.h"
#include "client_ascii_visual.h"
#include "../client_zone.h"
#include "../client_camera.h"
#include "Engine/Client/Scene/layer_manager.h"
#include "Engine/Client/Scene/layer.h"

using namespace Totem;
using namespace clan;

ClientAsciiVisual::ClientAsciiVisual(GameObject *owner, const std::string &name, ClientZone *zone, const LayerManagerPtr &layer_manager)
: AsciiVisual(owner, name), layer_manager(layer_manager)
{
	position_property = owner->add<Vec2i>("Position", Vec2i());

	character_layer = layer_manager->get("Character");
	dungeon_layer = layer_manager->get("Dungeon");

	slots.connect(zone->sig_draw, this, &ClientAsciiVisual::on_draw);
}

ClientAsciiVisual::~ClientAsciiVisual()
{
}

void ClientAsciiVisual::update(const float &delta_time)
{
}

void ClientAsciiVisual::on_draw(const ClientCameraPtr &camera)
{
	int character = '@';
	switch(visual_property.get())
	{
	case GOV_CHARACTER:
		character = '@'; break;
	case GOV_ORC:
		character = 'o'; break;
	case GOV_TROLL:
		character = 'T'; break;
	case GOV_ITEM:
		character = '['; break;
	case GOV_SCROLL:
		character = '#'; break;
	case GOV_POTION:
		character = '!'; break;
	case GOV_CORPSE:
		character = '%'; break;
	};

	auto view_space_position = position_property.get() - camera->get_view() + camera->get_projection();
	auto tile = std::make_shared<TileData>(color_background_property, color_foreground_property, character);
	character_layer->set_tile(view_space_position, tile);
}

