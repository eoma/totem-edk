#include "ComponentFactory.h"

#include "Components/Actor.h"
#include "Components/Visual.h"
#include "Components/Ai.h"

using namespace Totem;

ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
}

std::shared_ptr<IComponent<>> ComponentFactory::instantiate(const std::shared_ptr<Entity> &owner, const std::string &componentName)
{
	if(componentName.compare(Actor::GetType()) == 0)
		return owner->addComponent<Actor>( std::make_shared<Actor>(owner.get()) );
	else if(componentName.compare(Visual::GetType()) == 0)
		return owner->addComponent<Visual>( std::make_shared<Visual>(owner.get()) );
	else if(componentName.compare(Ai::GetType()) == 0)
		return owner->addComponent<Ai>( std::make_shared<Ai>(owner.get()) );

	return nullptr;
}

bool ComponentFactory::treatActor(const std::shared_ptr<Totem::IComponent<>> &component, const Parser::StatsInfo &statsInfo, const Parser::EntitiesInfo::EntityInfo &entityInfo)
{
	if(IComponent<>::isType<Actor>(component))
	{
		auto actor = std::static_pointer_cast<Actor>(component);
		return actor->initialize(statsInfo, entityInfo);
	}

	return false;
}
