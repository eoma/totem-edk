#include "Persistent.h"

#include "../Components/Map.h"
void Map::save(TCODZip &zip)
{
	zip.putInt(seed);
	for(unsigned int i = 0; i < width*height; i++)
	{
		zip.putInt((int)tiles[i].explored);
		zip.putInt((int)tiles[i].scent);
	}
}

void Map::load(TCODZip &zip)
{
	seed = zip.getInt();
	init(false);

	for(unsigned int i = 0; i < width*height; i++)
	{
		tiles[i].explored = (bool)zip.getInt();
		tiles[i].scent = (unsigned int)zip.getInt();
	}
}

#include "../Components/Gui.h"
void Gui::save(TCODZip &zip)
{
	zip.putInt((int)log.size());
	for(unsigned int i = 0; i < log.size(); i++)
	{
		zip.putString(log[i]->text.c_str());
		zip.putColor(&log[i]->col);
	}
}

void Gui::load(TCODZip &zip)
{
	int num_messages = zip.getInt();
	for(int i = 0; i < num_messages; i++)
	{
		std::string text = zip.getString();
		auto color = zip.getColor();
		message(color, text.c_str());
	}
}

#include "../Entity.h"
void Entity::save(TCODZip &zip)
{
	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		auto property = (*it).second;

		//Only store if property was changed from default value
		//if(property->isDirty())
		{
			zip.putString(property->getName().c_str());
			if(Totem::IProperty::isType<bool>(property))
			{
				auto value = std::static_pointer_cast<Totem::Property<bool>>(property)->get();
				zip.putInt((int)value);
			}
			else if(Totem::IProperty::isType<int>(property))
			{
				auto value = std::static_pointer_cast<Totem::Property<int>>(property)->get();
				zip.putInt(value);
			}
			else if(Totem::IProperty::isType<Vec2i>(property))
			{
				auto value = std::static_pointer_cast<Totem::Property<Vec2i>>(property)->get();
				zip.putInt(value.y());
				zip.putInt(value.x());
				std::cout << "Serialize " << name << "'s " << property->getName() << " to: (" << value.x() << ", " << value.y() << ")." << std::endl;
			}
			else if(Totem::IProperty::isType<float>(property))
			{
				auto value = std::static_pointer_cast<Totem::Property<float>>(property)->get();
				zip.putFloat(value);
			}
			else if(Totem::IProperty::isType<std::string>(property))
			{
				auto value = std::static_pointer_cast<Totem::Property<std::string>>(property)->get();
				zip.putString(value.c_str());
			}
			else if(Totem::IProperty::isType<TCODColor>(property))
			{
				auto value = std::static_pointer_cast<Totem::Property<TCODColor>>(property)->get();
				zip.putColor(&value);
			}
		}
	}

	for(auto it = propertyLists.begin(); it != propertyLists.end(); ++it)
	{
		auto list = (*it).second;

		//If type is EntityPtr
		if(Totem::IPropertyList::isType<EntityPtr>(list))
		{
			auto entity_list = std::static_pointer_cast<Totem::PropertyList<EntityPtr>>(list)->get();
			zip.putInt(entity_list.size());
			for(unsigned int i = 0; i < entity_list.size(); i++)
			{
				auto entity = entity_list[i];
				zip.putString(entity->getName().c_str());
				entity->save(zip);
			}
		}
	}
}

#include "../Engine.h"
#include "../Systems/PrefabSystem.h"
void Entity::load(TCODZip &zip)
{
	//This loader expects that the components are loaded in the exact same order, and has the same number of properties, as when it was saved!
	for(auto it = properties.begin(); it != properties.end(); ++it)
	{
		auto property = (*it).second;
		std::string stored_name = zip.getString();
		if(property->getName() != stored_name)
			continue;
		
		if(Totem::IProperty::isType<bool>(property))
		{
			std::static_pointer_cast<Totem::Property<bool>>(property)->set((bool)zip.getInt(), false);
		}
		else if(Totem::IProperty::isType<int>(property))
		{
			std::static_pointer_cast<Totem::Property<int>>(property)->set(zip.getInt(), false);
		}
		else if(Totem::IProperty::isType<Vec2i>(property))
		{
			std::static_pointer_cast<Totem::Property<Vec2i>>(property)->set(Vec2i(zip.getInt(), zip.getInt()), false);
			auto prop = std::static_pointer_cast<Totem::Property<Vec2i>>(property);
			std::cout << "Deserialize " << name << "'s " << stored_name << " to: (" << prop->get().x() << ", " << prop->get().y() << ")." << std::endl;
		}
		else if(Totem::IProperty::isType<float>(property))
		{
			std::static_pointer_cast<Totem::Property<float>>(property)->set(zip.getFloat(), false);
		}
		else if(Totem::IProperty::isType<std::string>(property))
		{
			std::static_pointer_cast<Totem::Property<std::string>>(property)->set(zip.getString(), false);
		}
		else if(Totem::IProperty::isType<TCODColor>(property))
		{
			std::static_pointer_cast<Totem::Property<TCODColor>>(property)->set(zip.getColor(), false);
		}
	}

	for(auto it = propertyLists.begin(); it != propertyLists.end(); ++it)
	{
		auto list = (*it).second;

		//If type is EntityPtr
		if(Totem::IPropertyList::isType<EntityPtr>(list))
		{
			auto entity_list = std::static_pointer_cast<Totem::PropertyList<EntityPtr>>(list);
			int list_size = zip.getInt();
			for(int i = 0; i < list_size; i++)
			{
				std::string entity_name = zip.getString();
				auto entity = Engine::getSingleton()->getPrefabSystem()->instantiate(entity_name);
				if(entity)
				{
					entity->load(zip);
					entity_list->push_back(entity);
				}
				else
				{
					std::cout << "ERROR! Failed to load from save file. Please delete the save file and start the game again." << std::endl;
					return;
				}
			}
		}
	}
}
