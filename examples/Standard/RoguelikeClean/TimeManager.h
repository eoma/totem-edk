#pragma once

class Entity;

#include <vector>
#include <memory>

class TimeManager
{
public:
	TimeManager();
	~TimeManager();

	void startPlayer(int duration, const std::shared_ptr<Entity> &entity);
	void start(int duration, const std::shared_ptr<Entity> &entity);
	void startAt(int start, int duration, const std::shared_ptr<Entity> &entity);

	void update();

	bool isReadyForNewTurn();

private:
	int _timeline;
	
	struct TimeEntry
	{
		int startTime;
		int currentTime;
		int durationTime;
		std::shared_ptr<Entity> entity;
		TimeEntry(int start, int duration, const std::shared_ptr<Entity> &entity) : startTime(start), currentTime(0), durationTime(duration), entity(entity) {}
	};

	std::shared_ptr<TimeEntry> _playerTime;
	std::vector<std::shared_ptr<TimeEntry>> _timeEntries;
};
