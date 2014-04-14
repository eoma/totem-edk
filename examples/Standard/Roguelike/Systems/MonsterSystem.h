#pragma once

#include "../Components/Monster.h"
#include "../Entity.h"
#include "../Engine.h"
#include "PrefabSystem.h"

#include <memory>
#include <iostream>
#include <vector>

class MonsterSystem;
typedef std::shared_ptr<MonsterSystem> MonsterSystemPtr;

class MonsterSystem : public sigslot::has_slots<>
{
public:
	MonsterSystem() {}
	~MonsterSystem() { std::cout << "Monster System is being destroyed!" << std::endl; }

	void add(Monster *monster, MonsterFamily family)
	{
		for(unsigned int i = 0; i < monsters.size(); i++)
		{
			if(monsters[i] == monster)
				return;
		}
		monsters.push_back(monster);
		std::cout << "Added monster " << monster->getOwner()->getName() << " to Monster System!" << std::endl;
	}
	void remove(Monster *monster)
	{
		for(unsigned int i = 0; i < monsters.size(); i++)
		{
			if(monsters[i] == monster)
			{
				//std::cout << "Removed monster from Monster System!" << std::endl;
				monsters[i] = monsters.back();
				monsters.pop_back();
				return;
			}
		}
	}

	void think()
	{
	}

	void createFamilyPrefab(const std::string &name, MonsterFamily family, int level)
	{
		for(unsigned int i = 0; i < families.size(); i++)
		{
			if(families[i].family == family)
			{
				families[i].findOrCreate(level).push_back(name);
				return;
			}
		}

		FamilyPrefab prefab(family);
		prefab.findOrCreate(level).push_back(name);
		families.push_back(prefab);
	}

	EntityPtr instantiate(MonsterFamily family, int level)
	{
		for(unsigned int i = 0; i < families.size(); i++)
		{
			if(families[i].family == family)
			{
				auto names = families[i].findOrCreate(level);
				if(names.empty())
					return nullptr;
				
				auto rng = TCODRandom::getInstance();
				auto index = rng->getInt(0, names.size()-1);
				return Engine::getSingleton()->getPrefabSystem()->instantiate(names[index]);
			}
		}
		return nullptr;
	}

private:
	std::vector<Monster*> monsters;

	struct FamilyRank
	{
		FamilyRank(int level) : level(level) {}
		
		int level;
		std::vector<std::string> names;
	};

	struct FamilyPrefab
	{
		FamilyPrefab(MonsterFamily family) : family(family) {}
		
		MonsterFamily family;
		std::vector<FamilyRank> levels;
		
		std::vector<std::string> &findOrCreate(int level)
		{
			for(unsigned int i = 0; i < levels.size(); i++)
			{
				if(levels[i].level == level)
					return levels[i].names;
			}
			levels.push_back(FamilyRank(level));
			return levels[levels.size()-1].names;
		}
	};

	std::vector<FamilyPrefab> families;
};