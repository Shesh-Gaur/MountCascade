#include "ZoomTrigger.h"
#include "ECS.h"
#include "Utilities.h"
float newFov;
void ZoomTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void ZoomTrigger::OnEnter()
{
	Trigger::OnEnter();
	//for (int i = 0; i < m_targetEntities.size(); i++)
	//{
	if (newFov > 60)
	{
		newFov -= 0.5f * Timer::deltaTime;

		ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(newFov, ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetAspect(), ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetNear(), 1000.f);
		ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());


	}
		//ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).ScaleBody(1.2f, 0, true);
	//}
}

void ZoomTrigger::OnExit()
{
	//ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(70, ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetAspect(), ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetNear(), 1000.f);

	Trigger::OnExit();
	

}
