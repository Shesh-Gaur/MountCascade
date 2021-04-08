#include "Player.h"

float dashtimer;
bool dashStarted = false;

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
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
	m_animController->AddAnimation(animations["IdleLeft"].get<Animation>());
	//Idle Right
	m_animController->AddAnimation(animations["IdleRight"].get<Animation>());

	//Run Animations\\

	//RunLeft
	m_animController->AddAnimation(animations["RunLeft"].get<Animation>());
	//RunRight
	m_animController->AddAnimation(animations["RunRight"].get<Animation>());

	//Attack Animations\\

	//AttackLeft
	m_animController->AddAnimation(animations["AttackLeft"].get<Animation>());
	//AttackRight
	m_animController->AddAnimation(animations["AttackRight"].get<Animation>());

	//Dash ANIMATIONS\\
	
	//Dash Left
	m_animController->AddAnimation(animations["DashLeft"].get<Animation>());
	//Dash Right
	m_animController->AddAnimation(animations["DashRight"].get<Animation>());

	//Charge Jump ANIMATIONS\\
	
	//Charge Jump Left
	m_animController->AddAnimation(animations["ChargeJumpLeft"].get<Animation>());
	//Charge Jump Right
	m_animController->AddAnimation(animations["ChargeJumpRight"].get<Animation>());

	//Jump ANIMATIONS\\
	
	//Jump Left
	m_animController->AddAnimation(animations["JumpLeft"].get<Animation>());
	//Jump Right
	m_animController->AddAnimation(animations["JumpRight"].get<Animation>());

	//Death ANIMATIONS\\
	
	//Death Left
	m_animController->AddAnimation(animations["DeathLeft"].get<Animation>());
	//Death Right
	m_animController->AddAnimation(animations["DeathRight"].get<Animation>());

	//Set Default Animation
	m_animController->SetActiveAnim(IDLERIGHT);

}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
}

void Player::MovementUpdate()
{
	m_moving = false;

	if (!m_dead) {
		if (m_hasPhysics)
		{
			float speed = 10.f;
			vec3 vel = vec3(0.f, 0.f, 0.f);

			if (Input::GetKey(Key::A))
			{
				vel = vel + vec3(-1.f, 0.f, 0.f);
				m_facing = LEFT;
				m_moving = true;

			}
			if (Input::GetKey(Key::D))
			{
				vel = vel + vec3(1.f, 0.f, 0.f);
				m_facing = RIGHT;
				m_moving = true;
			}
			m_physBody->SetVelocity(vel * speed);
		}
		else
		{
			//Regular Movement
			float speed = 15.f;

			if (Input::GetKey(Key::A))
			{
				//m_transform->SetPositionX(m_transform->GetPositionX() - (speed * Timer::deltaTime));
				m_facing = LEFT;
				m_moving = true;
			}
			if (Input::GetKey(Key::D))
			{
				//m_transform->SetPositionX(m_transform->GetPositionX() + (speed * Timer::deltaTime));
				m_facing = RIGHT;
				m_moving = true;
			}
		}
		if (Input::GetKeyDown(Key::F) && !m_dead)
		{
			m_moving = false;

			if (m_hasPhysics)
			{
				m_physBody->SetVelocity(vec3());
			}

			m_attacking = true;
			m_locked = true;
		}
	}
}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_dead)
	{
		activeAnimation = DEATH;
		ECS::GetComponent<PhysicsBody>(theAttackTrigger).SetPosition(b2Vec2(0, 0));
	}
	if (!m_dead) {

		if (m_moving)
		{
			//Puts it into the WALK category
			activeAnimation = RUN;
		}
		else if (m_attacking && haveYouPressedSpace == false)
		{
			activeAnimation = ATTACK;
			if (canYouFuckingJump == true)
			{
				//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());
				if (m_facing == RIGHT)
				{
					ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(3000.f, 0.f), true);

				}
				else
				{
					ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-3000.f, 0.f), true);

				}
			}
			else
			{

			}
			if (m_facing == RIGHT)
			{
				ECS::GetComponent<PhysicsBody>(theAttackTrigger).SetPosition(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition() + b2Vec2(23.f, 5.f));
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(1000.f, -55.f), true);


			}
			else
			{
				ECS::GetComponent<PhysicsBody>(theAttackTrigger).SetPosition(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition() + b2Vec2(-23.f, 5.f));
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-1000.f, -55.f), true);


			}
			
			//Check if the attack animation is done
			if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
			{
				ECS::GetComponent<PhysicsBody>(theAttackTrigger).SetPosition(b2Vec2(500,1000));
				if (canYouFuckingJump == true)
				{
					if (m_facing == RIGHT)
					{
						ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-100000.f, 0.f), true);
					}
					else
					{
						ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(100000.f, 0.f), true);
					}
				}
				//Will auto set to idle
				m_locked = false;
				m_attacking = false;
				//Resets the attack animation
				m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

				activeAnimation = IDLE;
			}
		}

		else
		{
			activeAnimation = IDLE;
		}
		if (!m_attacking && !haveYouPressedSpace && !canYouFuckingJump) activeAnimation = JUMP;

		if (haveYouPressedSpace) {
			activeAnimation = CHARGEJUMP;
			
			//if (!m_attacking && !haveYouPressedSpace && !canYouFuckingJump) activeAnimation = JUMP;
			//if (canYouFuckingJump) activeAnimation = RUN;
		}

		if (Input::GetKeyDown(Key::Shift)) {
			if (canDash) {
				dashStarted = true;
				m_dashing = true;
				activeAnimation = DASH;
				dashtimer += 1 * Timer::deltaTime;
				if (dashtimer > 0.5) {
					activeAnimation = RUN;
				}
			}
		}

		if (Input::GetKeyUp(Key::Shift)) {
			if (canDash) {
				dashtimer = 0;
				dashStarted = false;
			}
		}

		if (activeAnimation != DASH) m_dashing = false;

		
	}

	SetActiveAnimation(activeAnimation + (int)m_facing);
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
