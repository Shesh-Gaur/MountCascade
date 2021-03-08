#pragma once
#include "Trigger.h"
#include <Box2D/Box2D.h>

//Yo ECS.h is where you include new trigger types future me
class AttackTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;

	

protected:
	bool triggered = false;

};

