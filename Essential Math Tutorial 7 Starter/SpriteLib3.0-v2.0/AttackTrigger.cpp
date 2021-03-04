#include "AttackTrigger.h"
#include "AttackTrigger.h"
#include "ECS.h"
#include "Utilities.h"

void AttackTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void AttackTrigger::OnEnter()
{
	Trigger::OnEnter();
	
	//Code currently runs multiple times if multiple enemies are in the trigger. This crashes the game as it can check an entity that doesn't exist after it dies. (i think)
	int currentEntity = (int)ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData();

		ECS::GetComponent<PhysicsBody>(currentEntity).TakeDamage(1, currentEntity);
		ECS::GetComponent<PhysicsBody>(currentEntity).isKnocked();
		std::cout << "\n" << ECS::GetComponent<PhysicsBody>(currentEntity).GetName() << ": " << ECS::GetComponent<PhysicsBody>(currentEntity).GetHealth() << " HP";
	

}

void AttackTrigger::OnExit()
{

	Trigger::OnExit();


}
