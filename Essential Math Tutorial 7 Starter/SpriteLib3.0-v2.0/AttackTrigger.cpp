#include "AttackTrigger.h"
#include "ECS.h"
#include "Utilities.h"

int previousEntity;
void AttackTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void AttackTrigger::OnEnter()
{
	Trigger::OnEnter();
	
	int currentEntity = (int)ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData();
	//Code currently runs multiple times if multiple enemies are in the trigger. This if statement makes sure the same entity isn't affected twice, or after they're already killed.
	if (previousEntity != currentEntity)
	{
		ECS::GetComponent<PhysicsBody>(currentEntity).TakeDamage(1, currentEntity);

		ECS::GetComponent<PhysicsBody>(currentEntity).isKnocked();
		std::cout << "\n" << ECS::GetComponent<PhysicsBody>(currentEntity).GetName() << " " << currentEntity << ": " << ECS::GetComponent<PhysicsBody>(currentEntity).GetHealth() << " HP";

		previousEntity = currentEntity;
	}

}

void AttackTrigger::OnExit()
{

	Trigger::OnExit();
	previousEntity = 6969;

}
