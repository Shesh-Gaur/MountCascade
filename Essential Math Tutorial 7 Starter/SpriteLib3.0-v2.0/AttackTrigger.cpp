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

	//std::cout << "\n" <<(int)ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData();
	ECS::GetComponent<PhysicsBody>((int)ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData()).TakeDamage(1, (int)ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData());
	std::cout << "\n" << ECS::GetComponent<PhysicsBody>((int)ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData()).GetHealth();


}

void AttackTrigger::OnExit()
{

	Trigger::OnExit();


}
