#pragma once
#include "Trigger.h"
#include <Box2D/Box2D.h>

class JumpPowerupTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;


protected:
	bool triggered = false;

};
