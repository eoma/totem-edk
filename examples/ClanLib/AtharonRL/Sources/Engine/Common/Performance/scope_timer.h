
#pragma once

#include <intrin.h>

class ScopeTimerResult
{
public:
	ScopeTimerResult(const char *name, unsigned __int64 ticks) : name(name), ticks(ticks) { }

	const char *name;
	unsigned __int64 ticks;
};

class ScopeTimerResults
{
public:
	static void start();
	static void add_result(const ScopeTimerResult &result);
	static void end();
	static int percentage(const char *name);
	static std::string timer_results();

private:
	ScopeTimerResults();

	unsigned __int64 start_time;
	std::vector<ScopeTimerResult> results;
	unsigned __int64 end_time;

	unsigned __int64 current_start_time;
	std::vector<ScopeTimerResult> current_results;
	unsigned __int64 current_end_time;

	static ScopeTimerResults instance;
};

class ScopeTimer
{
public:
	ScopeTimer(const char *name)
	: name(name), start(__rdtsc())
	{
	}

	~ScopeTimer()
	{
		ScopeTimerResults::add_result(ScopeTimerResult(name, __rdtsc() - start));
	}

private:
	const char *name;
	unsigned __int64 start;
};

#define ScopeTimeFunction() ScopeTimer scope_timer(__FUNCTION__);
