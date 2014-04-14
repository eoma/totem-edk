#include "PrefabSystem.h"
#include "../Entity.h"
#include "../Engine.h"

#include "../Components/Actor.h"
#include "../Components/Map.h"
#include "../Components/Destructible.h"
#include "../Components/Attacker.h"
#include "../Components/Player.h"
#include "../Components/Monster.h"
#include "../Components/Gui.h"
#include "../Components/Container.h"
#include "../Components/Consumable.h"
#include "../Components/Pickable.h"
#include "../Components/Magic/Effect.h"
#include "../Components/Magic/Healer.h"
#include "../Components/Magic/Weave.h"
#include "../Utils/TargetSelector.h"

#include <libtcod.hpp>

PrefabSystem::PrefabSystem() 
{
}
PrefabSystem::~PrefabSystem() 
{
	for(unsigned int i = 0; i < prefabs.size(); i++)
		delete prefabs[i];
}

void PrefabSystem::createPrefab(const std::string &name, const std::vector<std::string> &components, const std::vector<Totem::IProperty*> &properties, const std::vector<Totem::IProperty*> &special_properties)
{
	prefabs.push_back(new EntityPrefab(name, components, properties, special_properties));
	std::cout << "Added new prefab: " << name << std::endl;
}

EntityPtr PrefabSystem::instantiate(const std::string &prefab_name)
{
	EntityPrefab *prefab = nullptr;
	for(unsigned int i = 0; i < prefabs.size(); i++)
	{
		if(prefabs[i]->getName() == prefab_name)
		{
			prefab = prefabs[i];
			break;
		}
	}
	if(prefab == nullptr)
		return nullptr;

	auto components = prefab->getComponents();
	auto properties = prefab->getProperties();
	auto specials = prefab->getSpecialProperties();

	//Make the new entity instance
	auto entity = std::make_shared<Entity>(prefab_name);
	
	//Fill it with components
	for(unsigned int i = 0; i < components.size(); i++)
		addComponent(entity, components[i], specials);
	
	//Adjust it's properties
	for(unsigned int i = 0; i < properties.size(); i++)
		addProperty(entity, properties[i]);

	//Do some special handling
	for(unsigned int i = 0; i < specials.size(); i++)
		addSpecialProperty(entity, specials[i]);
	
	//Add the new entity to the engine
	Engine::getSingleton()->add(entity);

	//return the new entity, fresh out from the oven!
	return entity;
}

void PrefabSystem::addComponent(EntityPtr entity, const std::string &name, const std::vector<Totem::IProperty*> &specials)
{
	auto engine = Engine::getSingleton();
	auto render_system = engine->getRenderSystem();

	if(name == "Actor")
	{
		entity->addComponent(std::make_shared<Actor>(entity, render_system));
	}
	else if(name == "Ai")
	{
		//This component doesn't do anything yet... and might not ever do anything at all, 
		//since Player and Monster components takes care of that logic right now. Might
		//change that in the future though, as common behavior related to intelligence
		//could merge in this component.
	}
	else if(name == "Attacker")
	{
		entity->addComponent(std::make_shared<Attacker>(entity));
	}
	else if(name == "Consumable")
	{
		entity->addComponent(std::make_shared<Consumable>(entity));
	}
	else if(name == "Container")
	{
		entity->addComponent(std::make_shared<Container>(entity));
	}
	else if(name == "Destructible")
	{
		entity->addComponent(std::make_shared<Destructible>(entity, render_system));
	}
	else if(name == "Monster")
	{
		Totem::IProperty *property = nullptr;
		for(unsigned int i = 0; i < specials.size(); i++)
		{
			if(specials[i]->getName() == "Family")
			{
				property = specials[i];
				break;
			}
		}
		if(property && Totem::IProperty::isType<int>(*property))
			entity->addComponent(std::make_shared<Monster>(entity, (MonsterFamily)static_cast<Totem::Property<int>*>(property)->get(), engine->getMonsterSystem()));
		else
			entity->addComponent(std::make_shared<Monster>(entity, GOBLIN_FAMILY, engine->getMonsterSystem()));
	}
	else if(name == "Pickable")
	{
		entity->addComponent(std::make_shared<Pickable>(entity, render_system));
	}
	else if(name == "Player")
	{
		entity->addComponent(std::make_shared<Player>(entity, render_system));
	}
		//Magic
	else if(name == "Effect")
	{
		entity->addComponent(std::make_shared<Effect>(entity));
	}
	else if(name == "Healer")
	{
		entity->addComponent(std::make_shared<Healer>(entity));
	}
	else if(name == "Weave")
	{
		entity->addComponent(std::make_shared<Weave>(entity));
	}
}

//Could add many more types to this, but for now we just cover the requirements of the EntityParser
void PrefabSystem::addProperty(EntityPtr entity, Totem::IProperty *property)
{
	auto name = property->getName();

	//A component must already have added this property to the entity at this point...
	if(entity->hasProperty(name) == false)
		return;

	if(Totem::IProperty::isType<bool>(*property))
		entity->get<bool>(name) = static_cast<Totem::Property<bool>*>(property)->get();
	else if(Totem::IProperty::isType<int>(*property))
		entity->get<int>(name) = static_cast<Totem::Property<int>*>(property)->get();
	else if(Totem::IProperty::isType<float>(*property))
		entity->get<float>(name) = static_cast<Totem::Property<float>*>(property)->get();
	else if(Totem::IProperty::isType<std::string>(*property))
		entity->get<std::string>(name) = static_cast<Totem::Property<std::string>*>(property)->get();
	else if(Totem::IProperty::isType<TCODColor>(*property))
		entity->get<TCODColor>(name) = static_cast<Totem::Property<TCODColor>*>(property)->get();
}

//Here we handle the kind of properties that requires some special attention
void PrefabSystem::addSpecialProperty(EntityPtr entity, Totem::IProperty *property)
{
	auto name = property->getName();
	if(name == "TargetSelector" && Totem::IProperty::isType<std::string>(*property))
	{
		if(entity->hasComponent<Effect>())
		{
			auto effect = entity->getComponent<Effect>();
			auto selector_type = static_cast<Totem::Property<std::string>*>(property)->get();

			if(selector_type == "ClosestMonster")
				effect->setSelector(std::make_shared<TargetSelector>(TargetSelector::CLOSEST_MONSTER));
			else if(selector_type == "SelectedMonster")
				effect->setSelector(std::make_shared<TargetSelector>(TargetSelector::SELECTED_MONSTER));
			else if(selector_type == "SelectedRange")
				effect->setSelector(std::make_shared<TargetSelector>(TargetSelector::SELECTED_RANGE));
		}
	}
	else if(name == "TargetRange" && Totem::IProperty::isType<float>(*property))
	{
		if(entity->hasComponent<Effect>())
		{
			auto effect = entity->getComponent<Effect>();

			//Note that TargetSelector property must have been defined before this property is set...
			if(effect->getSelector())
				effect->getSelector()->setRange(static_cast<Totem::Property<float>*>(property)->get());
		}
	}
}
