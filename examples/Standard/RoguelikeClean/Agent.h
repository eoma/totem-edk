#pragma once

#include "Controller.h"

class Agent : public Controller
{
public:
	Agent();
	virtual ~Agent();

	int estimateAction(unsigned int index = 0) override;
	void takeAction(unsigned int index = 0) override;

protected:
	void _internalThink(int elapsedTime) override;

private:
	void _handleInput();
	void _handleActionKeyInput(int ascii);
};