#include "DashTextTrigger.h"
#include "ECS.h"
#include "Utilities.h"

//int previousEntity;

//int dashTextEntity;

bool isDTTRan;

void DashTextTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void DashTextTrigger::OnEnter()
{
	Trigger::OnEnter();

	//std::cout << "TRIGGER ACTIVATED!";

	if(!isDTTRan) { //Dash Text Box
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//dashTextEntity = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "tutorial/Chatbox-Dash.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 96, 48);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(380.f, 155.f, 2.f));

		isDTTRan = true;
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

void DashTextTrigger::OnExit()
{

	Trigger::OnExit();

	//std::cout << "TRIGGER DEACTIVATED!";
	//previousEntity = 6969;

}
