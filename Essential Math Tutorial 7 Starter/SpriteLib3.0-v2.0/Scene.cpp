#include "Scene.h"
#include "Utilities.h"
#include "RayCastCallback.h"

Scene::Scene(std::string name)
{
	m_physicsWorld = new b2World(m_gravity);
	m_name = name;
}

void Scene::Unload()
{
	if (m_sceneReg != nullptr)
	{
		delete m_sceneReg;
		m_sceneReg = nullptr;
	}

	if (m_physicsWorld != nullptr)
	{
		delete m_physicsWorld;
		m_physicsWorld = nullptr;
	}
}

void Scene::InitScene(float windowWidth, float windowHeight)
{
	//Default scene class init just includes a camera so stuff doesn't immediately crash
	printf("You shouldn't be running this, while initializing this Scene Type.\n");

	//Dynamically allocates the register
	if (m_sceneReg == nullptr)
	{
		m_sceneReg = new entt::registry;
	}

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
	}

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		m_helloWorldSign = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}
}

void Scene::Update()
{
	auto& tempSpr = m_sceneReg->get<Sprite>(m_helloWorldSign);
	
	tempSpr.SetTransparency((0.5 * sin(Timer::time * 3.f)) + 0.5f);
}

void Scene::GUI()
{
	//Does nothin

}

void Scene::AdjustScrollOffset()
{
	float maxSizeX = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetOrthoSize().y;
	float maxSizeY = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetOrthoSize().w;

	float playerHalfSize = ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).GetWidth() / 2.f;

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset((maxSizeX * BackEnd::GetAspectRatio()) - playerHalfSize);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(maxSizeY - playerHalfSize);
}

void Scene::CreateCameraEntity(bool mainCamera, float windowWidth, float windowHeight, float left, float right, float bottom, float top, 
									float zNear, float zFar, float aspectRatio, bool vertScroll, bool horizScroll)
{
	//Setup main camera
	{
		//Creates Camera Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, false);

		ECS::AttachComponent<Camera>(entity);
		if (horizScroll)
		{
			ECS::AttachComponent<HorizontalScroll>(entity);
		}
		if (vertScroll)
		{
			ECS::AttachComponent<VerticalScroll>(entity);
		}

		vec4 temp = vec4(left, right, bottom, top);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, zNear, zFar);

		if (horizScroll)
		{
			ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		}
		if (vertScroll)
		{
			ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		}
	}
}

entt::registry* Scene::GetScene() const
{
	return m_sceneReg;
}

void Scene::SetScene(entt::registry& scene)
{
	m_sceneReg = &scene;
}

vec4 Scene::GetClearColor() const
{
	return m_clearColor;
}

void Scene::SetClearColor(vec4 clearColor)
{
	m_clearColor = clearColor;
}

std::string Scene::GetName() const
{
	return m_name;
}

void Scene::SetName(std::string name)
{
	m_name = name;
}

b2Vec2 Scene::GetGravity() const
{
	return m_gravity;
}

void Scene::SetGravity(b2Vec2 grav)
{
	m_gravity = grav;
}

b2World & Scene::GetPhysicsWorld()
{
	return *m_physicsWorld;
}

void Scene::SetWindowSize(float windowWidth, float windowHeight)
{
	//TODO: Find new way to get the main camera
	auto& tempCam = m_sceneReg->get<Camera>(MainEntities::MainCamera());
	
	tempCam.SetWindowSize(vec2(windowWidth, windowHeight));
	tempCam.Orthographic(float(windowWidth / windowHeight), tempCam.GetOrthoSize().x, tempCam.GetOrthoSize().y,
															tempCam.GetOrthoSize().z, tempCam.GetOrthoSize().w,
															tempCam.GetNear(), tempCam.GetFar());
}

void Scene::CheckTransition()
{
	if (MainEntities::MainLoading() != 0)
	{
		if (ECS::GetComponent<Sprite>(MainEntities::MainLoading()).GetTransparency() >= 1.f)
		{

			ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetLinearVelocity(b2Vec2(0.f, -10.f));


		}
		else if (ECS::GetComponent<Sprite>(MainEntities::MainLoading()).GetTransparency() >= 0.1f)
		{
			if (transitionStarted == false)
			{

				playerLoadingPos = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition() + b2Vec2(0.f, 50.f);
				transitionStarted = true;
			}
			ECS::GetComponent<Sprite>(MainEntities::MainLoading()).SetTransparency(ECS::GetComponent<Sprite>(MainEntities::MainLoading()).GetTransparency() + 2.f * Timer::deltaTime);
			ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(playerLoadingPos);
		}
	}
}

void Scene::makeLoadingScreen()
{

		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainLoading(entity, true);
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "black.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2920, 2080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1000.f, -150.f, -60.f));
	


}

void Scene::PlayerMovement()
{
	//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetGravityScale(-m_gravity.y * Timer::deltaTime);

	if (loadStarted == false) {
		loadStarted = true;
	}


	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	float speed = 10000000.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	float maxVel = 75.f;

	if (Input::GetKey(Key::A) && !Input::GetKey(Key::D) && health > 0)
	{
		//std::cout << "\nVELOCITY" << abs(player.GetBody()->GetLinearVelocity().x);

		//player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * Timer::deltaTime * speed, 0.f), true);
		player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-speed * Timer::deltaTime, 0), true);

		if (player.GetBody()->GetLinearVelocity().x < -maxVel)
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(-maxVel, player.GetBody()->GetLinearVelocity().y));


		}

		RayCastCallback checkRay;
		m_physicsWorld->RayCast(&checkRay, player.GetPosition() + b2Vec2(-13, 0), player.GetPosition() + b2Vec2(-15, 0));

		if (checkRay.m_fixture == nullptr)
		{
			m_physicsWorld->RayCast(&checkRay, player.GetPosition() + b2Vec2(-13, 27), player.GetPosition() + b2Vec2(-15, 27));
			if (checkRay.m_fixture == nullptr)
			{
				m_physicsWorld->RayCast(&checkRay, player.GetPosition() + b2Vec2(-13, -25), player.GetPosition() + b2Vec2(-15, -25));
			}
		}

		if (checkRay.m_fixture != nullptr && checkRay.m_fixture->GetBody()->GetType() == b2_staticBody)
		{
			if (ECS::GetComponent<PhysicsBody>((int)checkRay.m_fixture->GetBody()->GetUserData()).GetName() == "Ice Wall")
			{

				player.GetBody()->SetLinearVelocity(b2Vec2(player.GetBody()->GetLinearVelocity().x, -10));
				canJump.m_canJump = false;
				isTouchingIceWall = true;

			}
			player.GetBody()->GetFixtureList()->SetFriction(0);
		}
		else
		{
			//player.GetBody()->GetFixtureList()->SetFriction(1.1f);
		}

		if (Input::GetKeyDown(Key::Shift))
		{
			if (airDashCounter >= 1)
			{
				player.GetBody()->SetLinearVelocity(b2Vec2(0, 0));

				RayCastCallback dashRay;
				m_physicsWorld->RayCast(&dashRay, player.GetBody()->GetPosition(), player.GetBody()->GetPosition() + b2Vec2(-dashAmount, 0));
				if (dashRay.m_fixture != NULL)
				{
					player.SetPosition(dashRay.m_point + b2Vec2(20.f, 0));
				}
				else
				{
					player.SetPosition(player.GetBody()->GetPosition() + b2Vec2(-dashAmount, 0));
				}
				airDashCounter -= 1;
			}

		}
	}
	if (Input::GetKey(Key::D) && !Input::GetKey(Key::A) && health > 0)
	{
		//std::cout << "\nVELOCITY" << abs(player.GetBody()->GetLinearVelocity().x);

		//player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * Timer::deltaTime * speed, 0.f), true);
		player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(speed * Timer::deltaTime, 0), true);
		if (player.GetBody()->GetLinearVelocity().x > maxVel)
		{
			player.GetBody()->SetLinearVelocity(b2Vec2(maxVel, player.GetBody()->GetLinearVelocity().y));
		}

		RayCastCallback checkRay;
		m_physicsWorld->RayCast(&checkRay, player.GetPosition() + b2Vec2(13, 0), player.GetPosition() + b2Vec2(15, 0));

		if (checkRay.m_fixture == nullptr)
		{
			m_physicsWorld->RayCast(&checkRay, player.GetPosition() + b2Vec2(13, 27), player.GetPosition() + b2Vec2(15, 27));
			if (checkRay.m_fixture == nullptr)
			{
				m_physicsWorld->RayCast(&checkRay, player.GetPosition() + b2Vec2(13, -25), player.GetPosition() + b2Vec2(15, -25));
			}
		}

		if (checkRay.m_fixture != nullptr && checkRay.m_fixture->GetBody()->GetType() == b2_staticBody)
		{
			if (ECS::GetComponent<PhysicsBody>((int)checkRay.m_fixture->GetBody()->GetUserData()).GetName() == "Ice Wall")
			{
				player.GetBody()->SetLinearVelocity(b2Vec2(player.GetBody()->GetLinearVelocity().x, -10));
				canJump.m_canJump = false;
				isTouchingIceWall = true;

			}
			player.GetBody()->GetFixtureList()->SetFriction(0);

		}
		else
		{
			//player.GetBody()->GetFixtureList()->SetFriction(1.1f);
		}


		if (Input::GetKeyDown(Key::Shift))
		{
			if (airDashCounter >= 1)
			{
				player.GetBody()->SetLinearVelocity(b2Vec2(0, 0));

				RayCastCallback dashRay;
				m_physicsWorld->RayCast(&dashRay, player.GetBody()->GetPosition(), player.GetBody()->GetPosition() + b2Vec2(dashAmount, 0));
				if (dashRay.m_fixture != NULL)
				{
					player.SetPosition(dashRay.m_point - b2Vec2(20.f, 0));
				}
				else
				{
					player.SetPosition(player.GetBody()->GetPosition() + b2Vec2(dashAmount, 0));

				}
				airDashCounter -= 1;
			}
		}

	}
//JUMP CODE
	RayCastCallback jumpRay;
	m_physicsWorld->RayCast(&jumpRay, player.GetPosition() + b2Vec2(0, -32), player.GetPosition() + b2Vec2(0, -36));

	if (jumpRay.m_fixture != NULL)
	{
		isTouchingIceWall = false;
	}


	if (canJump.m_canJump && !Input::GetKey(Key::D) && !Input::GetKey(Key::A)) {
		player.GetBody()->SetLinearVelocity(speed * vel + b2Vec2(player.GetBody()->GetLinearVelocity().x * 0.9f,
			player.GetBody()->GetLinearVelocity().y));
	}
	if (canJump.m_canJump)
	{
		spacePressed = false;

	}
	if (isTouchingIceWall == false)
	{

		if (canJump.m_canJump && health > 0)
		{
			if (airDashCounter <= airDashDefault && loadStarted == true)
			{
				airDashCounter += 2.f * Timer::deltaTime;
			}
			else
			{
				airDashCounter = airDashDefault;

			}

			if (Input::GetKeyDown(Key::Space))
			{
				spaceReleased = false;
				spacePressed = false;
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 180000.f), true);
				airJumpCounter = airJumpDefault;
				canJump.m_canJump = false;
			}
		}
		else
		{

			if (Input::GetKeyUp(Key::Space) && health > 0)
			{
				spaceReleased = true;
				if (player.GetBody()->GetLinearVelocity().y > 0)
				{
					player.GetBody()->SetLinearVelocity(b2Vec2(player.GetBody()->GetLinearVelocity().x / 2, player.GetBody()->GetLinearVelocity().y / 2));

				}

			}
			if (airJumpCounter > 0 && spaceReleased == true && health > 0)
			{
				if (Input::GetKey(Key::Space))
				{
					if (jumpForce < 250000)
					{
						player.GetBody()->SetLinearVelocity(b2Vec2(player.GetBody()->GetLinearVelocity().x / 2, player.GetBody()->GetLinearVelocity().y / 2));
						jumpForce += 60000 * Timer::deltaTime;
						spacePressed = true;
						//std::cout << "\n" << jumpForce;
						//Here
					}
					else
					{
						jumpForce = defaultForce;
						spacePressed = false;
						airJumpCounter--;
						canJump.m_canJump = false;
					}
				}
				if (Input::GetKeyUp(Key::Space) && spacePressed)
				{

					player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, jumpForce), true);
					jumpForce = defaultForce;
					canJump.m_canJump = false;
					airJumpCounter--;
				}

			}
		}
	}

}

