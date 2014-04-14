#include "TargetSelector.h"
#include "../Components/Map.h"
#include "../Components/Actor.h"
#include "../Components/Gui.h"
#include "../Engine.h"

TargetSelector::TargetSelector(SelectorType type)
	: type(type), range(1.0f)
{
}

TargetSelector::~TargetSelector()
{
}

std::vector<ActorPtr> TargetSelector::selectTargets(const ActorPtr &wearer)
{
	std::vector<ActorPtr> list;
	auto engine = Engine::getSingleton();
	auto gui = engine->getGui();

	switch(type) 
	{
        case CLOSEST_MONSTER :
        {
			auto closestMonster = engine->getClosestMonster(wearer->getPosition(), range);
            if ( closestMonster )
                list.push_back(closestMonster);
            
        }
        break;
		case SELECTED_MONSTER :
		{
			Vec2i targetPos;
			gui->message(TCODColor::cyan, "Left-click to select an enemy,\nor right-click to cancel.");
			if ( engine->pickATile(targetPos,range)) 
			{
				auto actor = engine->getActor(targetPos);
				if(actor)
					list.push_back(actor);
			}
		} break;
		case WEARER_RANGE :
		{
			auto actors = Engine::getSingleton()->getActors();
			for (unsigned int i = 0; i < actors.size(); i++)
			{
				auto actor = actors[i];
				if ( actor->isAlive() && wearer->getPosition().distance(actor->getPosition()) <= range)
					list.push_back(actor);
			}
		} break;
		case SELECTED_RANGE :
		{
			Vec2i targetPos;
			gui->message(TCODColor::cyan, "Left-click to select a tile,\nor right-click to cancel.");
			if ( engine->pickATile(targetPos)) 
			{
				auto actors = Engine::getSingleton()->getActors();
				for (unsigned int i = 0; i < actors.size(); i++)
				{
					auto actor = actors[i];
					
					//Let's not pick ourselves...
					if(actor->getOwner() == wearer->getOwner())
						continue;

					if ( actor->isAlive() && wearer->getPosition().distance(actor->getPosition()) <= range)
						list.push_back(actor);
				}
			}
		} break;
	};

	if(list.empty())
		gui->message(TCODColor::lightGrey,"No enemy is close enough");

	return list;
}
