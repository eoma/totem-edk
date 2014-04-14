#include "precomp.h"
#include "component_serializer.h"

#include "Engine/Common/GameWorld/Components/ascii_visual.h"
#include "Engine/Common/GameWorld/Components/transform.h"

using namespace Totem;
using namespace clan;

std::string ComponentSerializer::get_component_type(std::shared_ptr<Totem::IComponent<>> component)
{
	if(IComponent<>::isType<AsciiVisual>(component))
	{
		return AsciiVisual::getType();
	}
	else if(IComponent<>::isType<Transform>(component))
	{
		return Transform::getType();
	}
	else
	{
		throw Exception("ComponentSerializer::get_component_type - Unable to get type for component " + component->getRuntimeTypeId());
	}
}
