#pragma once

#include <Totem/Property.h>
#include <vector>
#include <memory>

class Entity; typedef std::shared_ptr<Entity> EntityPtr;

class PrefabSystem
{
public:
	PrefabSystem();
	~PrefabSystem();

	//This should be called from EntityParser, but is not restricted to only that...
	void createPrefab(const std::string &name, const std::vector<std::string> &components, const std::vector<Totem::IProperty*> &properties, const std::vector<Totem::IProperty*> &special_properties);

	//Call this in code to create an instance of a prefab. A prefab is defined in .cfg files loaded by the EntityParser.
	EntityPtr instantiate(const std::string &prefab_name);

private:
	class EntityPrefab
	{
	public:
		EntityPrefab(const std::string &name, const std::vector<std::string> &components, const std::vector<Totem::IProperty*> &properties, const std::vector<Totem::IProperty*> &special_properties) 
			: name(name) 
		{
			this->components.reserve(components.size());
			this->properties.reserve(properties.size());
			for(unsigned int i=0; i < components.size(); i++)
				this->components.push_back(components[i]);
			for(unsigned int i=0; i < properties.size(); i++)
				this->properties.push_back(properties[i]);
			for(unsigned int i=0; i < special_properties.size(); i++)
				this->special_properties.push_back(special_properties[i]);
		}
		~EntityPrefab() {}

		const std::string &getName() const { return name; }
		const std::vector<std::string> &getComponents() const { return components; }
		const std::vector<Totem::IProperty*> &getProperties() const { return properties; }
		const std::vector<Totem::IProperty*> &getSpecialProperties() const { return special_properties; }

	private:
		std::string name;
		std::vector<std::string> components;
		std::vector<Totem::IProperty*> properties;
		std::vector<Totem::IProperty*> special_properties;
	};

	void addComponent(EntityPtr entity, const std::string &name, const std::vector<Totem::IProperty*> &specials);
	void addProperty(EntityPtr entity, Totem::IProperty *property);
	void addSpecialProperty(EntityPtr entity, Totem::IProperty *property);

	std::vector<EntityPrefab*> prefabs;
};
typedef std::shared_ptr<PrefabSystem> PrefabSystemPtr;