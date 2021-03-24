#include "Bat.h"

Bat::Bat()
{
}

Bat::Bat(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitBat(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Bat::InitBat(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite,
	AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

	//IDLE ANIMATIONS\\
	
	//Idle Left
	m_animController->AddAnimation(animations["MoveRight"].get<Animation>());
	//Idle Right
	m_animController->AddAnimation(animations["MoveLeft"].get<Animation>());

	//Run Animations\\

	//RunLeft
	m_animController->AddAnimation(animations["DamageRight"].get<Animation>());
	//RunRight
	m_animController->AddAnimation(animations["DamageLeft"].get<Animation>());

	//Set Default Animation
	m_animController->SetActiveAnim(FLY_RIGHT);

}

void Bat::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void Bat::MovementUpdate()
{
	m_moving = false;

}

void Bat::AnimationUpdate()
{
	int activeAnimation = 0;

	
		if (m_moving)
		{
			//Puts it into the WALK category
			activeAnimation = FLY;
		}
		if (m_damaged) activeAnimation = DAMAGE;
			//Check if the attack animation is done
			if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
			{
				//Will auto set to idle
				m_locked = false;
				m_damaged = false;
				//Resets the attack animation
				m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

				activeAnimation = FLY;
			}


	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Bat::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
