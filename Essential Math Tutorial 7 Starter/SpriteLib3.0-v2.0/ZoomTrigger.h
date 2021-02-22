#pragma once
#include "Trigger.h"
#include <Box2D/Box2D.h>


class ZoomTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;

	b2Vec2 movement;

protected:
	bool triggered = false;

};

