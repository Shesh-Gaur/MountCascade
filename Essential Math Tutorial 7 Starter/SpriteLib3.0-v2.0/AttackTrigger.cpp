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

	std::cout << "\n" << ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData();
	//if (GetTriggerEntity() != MainEntities::MainPlayer())
	//{

	//	if (ECS::GetComponent<PhysicsBody>(GetTriggerEntity()).GetBody()->GetType() != b2_staticBody)
	//	{
	//		std::cout << "\nTHING DETECTED!";
	//	}
	//}

}

void AttackTrigger::OnExit()
{

	Trigger::OnExit();


}
