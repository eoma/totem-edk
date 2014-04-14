
#include "precomp.h"
#include "tick_timer.h"

using namespace clan;

TickTimer::TickTimer()
: last_tick_time(0)
{
}

void TickTimer::reset()
{
	last_tick_time = 0;
}

int TickTimer::update()
{
	unsigned int current_time = System::get_time();
	if (last_tick_time == 0 || current_time < last_tick_time)
	{
		last_tick_time = current_time;
		return 0;
	}
	else
	{
		unsigned int elapsed_time = current_time - last_tick_time;
		unsigned int ticks_available = elapsed_time / tick_length;
		last_tick_time += tick_length * ticks_available;
		return ticks_available;
	}
}
