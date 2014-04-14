#include "TimeManager.h"

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::startPlayer(int duration, const std::shared_ptr<Entity> &entity)
{

}

void TimeManager::start(int duration, const std::shared_ptr<Entity> &entity)
{

}

void TimeManager::startAt(int start, int duration, const std::shared_ptr<Entity> &entity)
{
}

void TimeManager::update()
{
}

bool TimeManager::isReadyForNewTurn()
{
	return _playerTime == nullptr;
}
