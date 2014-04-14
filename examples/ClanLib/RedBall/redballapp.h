
#pragma once

class Entity;

class RedBallApp
{
public:
	void run();

private:
	void on_window_close();
	void on_input_up(const CL_InputEvent &key);
	float calculate_delta_time();
	
	bool quit;
	unsigned int last_time;

	std::vector<Entity *> entities;

	CL_SlotContainer slots;
};
