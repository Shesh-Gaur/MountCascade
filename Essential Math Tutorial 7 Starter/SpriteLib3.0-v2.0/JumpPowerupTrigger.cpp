#include "JumpPowerupTrigger.h"

void JumpPowerupTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void JumpPowerupTrigger::OnEnter()
{
	Trigger::OnEnter();

	//std::cout << "TRIGGER ACTIVATED!";


	/*int currentEntity = (int)ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData();
	//Code currently runs multiple times if multiple enemies are in the trigger. This if statement makes sure the same entity isn't affected twice, or after they're already killed.
	if (previousEntity != currentEntity)
	{
		ECS::GetComponent<PhysicsBody>(currentEntity).TakeDamage(1, currentEntity);
		ECS::GetComponent<PhysicsBody>(currentEntity).isKnocked();
		std::cout << "\n" << ECS::GetComponent<PhysicsBody>(currentEntity).GetName() << " " << currentEntity << ": " << ECS::GetComponent<PhysicsBody>(currentEntity).GetHealth() << " HP";

		previousEntity = currentEntity;
	}*/

}

void JumpPowerupTrigger::OnExit()
{

	Trigger::OnExit();

	//std::cout << "TRIGGER DEACTIVATED!";
	//previousEntity = 6969;

}