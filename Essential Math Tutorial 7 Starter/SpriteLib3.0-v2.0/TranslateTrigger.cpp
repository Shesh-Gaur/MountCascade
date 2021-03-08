#include "TranslateTrigger.h"
#include "ECS.h"

void TranslateTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void TranslateTrigger::OnEnter()
{
	Trigger::OnEnter();

		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(movement,true);
		std::cout << "\n" << movement.x;
	
	
}

void TranslateTrigger::OnExit()
{
	Trigger::OnExit();
	
}
