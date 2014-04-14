#include "precomp.h"
#include "gameobject.h"

using namespace clan;

GameObject::GameObject(int id, const std::string &name)
: Totem::ComponentContainer<>(), Totem::PropertyContainer<>(), destroyed(false), id(id)
{
	property_name = add<std::string>("Name", name);
}

int GameObject::get_id() const
{ 
	return id; 
}

std::string GameObject::get_name() const
{ 
	return property_name.get();
}

void GameObject::update(float time_elapsed)
{
	updateComponents(time_elapsed);
	updateProperties();
}

bool GameObject::is_destroyed() const 
{ 
	return destroyed; 
}

void GameObject::destroy() 
{ 
	destroyed = true; 
}
