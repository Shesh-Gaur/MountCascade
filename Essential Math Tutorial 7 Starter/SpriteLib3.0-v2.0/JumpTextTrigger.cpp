#include "JumpTextTrigger.h"
#include "ECS.h"
#include "Utilities.h"

//int previousEntity;

bool isJTTRan;

void JumpTextTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void JumpTextTrigger::OnEnter()
{
	Trigger::OnEnter();


	//std::cout << "TRIGGER ACTIVATED!";

	if(!isJTTRan) { //Jump Text Box
	//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "ui/placeholder.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 96, 48);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 100.f, 2.f));

		isJTTRan = true;
	}

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

void JumpTextTrigger::OnExit()
{

	Trigger::OnExit();

	//std::cout << "TRIGGER DEACTIVATED!";
	//previousEntity = 6969;

}
