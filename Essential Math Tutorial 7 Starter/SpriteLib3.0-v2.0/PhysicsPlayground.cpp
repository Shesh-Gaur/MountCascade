#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "RayCastCallback.h"
#include <random>
#include <fstream>
#include <iostream>
#include "Astar.h"
#include "ZoomTrigger.h"
#include "ZoomConfirm.h"
#include "TransitionTrigger.h"


int startTime3 = clock();
int startTimeJumpBoost = clock();
int jumpBoostFrame = 0;
double diffTimeJumpBoost;
double diffTime3;

bool hasJumpBoostUnlocked = false;

bool activatePuni2 = false;
int puni2StartTime = 0;
double puni2DiffTime = 0;
int puni2CurFrame = 0;

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	
	//m_gravity = b2Vec2(0.f, -105.f);
	//m_physicsWorld->SetGravity(m_gravity);

	//m_physicsWorld->SetContactListener(&listener);
	


}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	m_physicsWorld = new b2World(m_gravity);
	m_name = "Mt.Cascade";
	m_gravity = b2Vec2(0.f, -235.f); //Use this if ya wanna change gravity
	m_physicsWorld->SetGravity(m_gravity);
	m_physicsWorld->SetContactListener(&listener);



	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	aRatio = aspectRatio;
	//EffectManager::CreateEffect(EffectType::Vignette, windowWidth, windowHeight);
	//EffectManager::CreateEffect(EffectType::Sepia, windowWidth, windowHeight);
	//EffectManager::RemoveEffect(EffectManager::GetSepiaHandle()); //added this to get rid of sephia

	{
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "overlay/The_Mushroom_Cave_Overlay.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 4096, 4096);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(525.f, 250.f, 0.021f));
	}

	{ //Punisher Text Entity

		auto entity = ECS::CreateEntity();
		puni2Text = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "tutorial/Chatbox-Punisher Healing Station.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 96, 48);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.0f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-600.f, 50.f, 0.01f));
	}

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);

		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		//ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<Camera>(entity).Perspective(fov, aspectRatio, 3.0f, 50.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	{
		auto entity = ECS::CreateEntity();
		puni2 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "punisher/Punisher-Cooking1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 128);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.0f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-750.f, 55.f, 0.01f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		editorEnabled = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "LevelEditorUI/Level Editor Enabled.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 240, 125);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
	}



	{ //dash bar
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		dashBar = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "LevelEditorUI/Level Editor Enabled.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 80, 42);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
	}

	{ //health bar
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		healthBar = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "ui/Health6.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 22, 5);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
	}

	{ //Jump Boost Collectable
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		jumpBoostEntity = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "jumpBoost/Boost1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(200.f, 80.f, 2.f));
	}



	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		playerFollow = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Set up the components
		std::string fileName = "BeachBall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0), float32(100));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, OBJECTS,PICKUP,0.f,0.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);
		tempPhsBody.SetGravityScale(0.f);

	}

	{ //Dash Text Box
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//dashTextEntity = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "tutorial/Chatbox-Charged Jump.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 96, 48);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(500.f, 155.f, 2.f));

	}

	{ //Dash Punisher
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//Creates entity
		auto entity = ECS::CreateEntity();

		//dashTextEntity = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "punisher/Punisher1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 77, 100);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(580.f, 165.f, 0.01f));

	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		rayMarker = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "LevelEditorUI/Cursor.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		changesSaved = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "LevelEditorUI/ChangesSaved.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 160,90);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 18.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		editZone = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "BeachBall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
	}
	
	//Player
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/Player.png";
		std::string animations = "Player.json";
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 64, 64, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 0.02f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		
		float shrinkX = 40.f;
		float shrinkY = 10.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-800.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -5.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 1.1f, 1.6f);
		//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
		//tempPhsBody = PhysicsBody(entity, BodyType::BOX, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
		tempPhsBody.SetName("Player");

	}
	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "tower.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 203);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.0f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-80.f, 20.f, 3.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "black.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10200, 5800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-700.f, 150.f, -70.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_back-background1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3200, 1800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(200.f, 150.f,-65.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_back-background3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3200, 1800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(200.f, 450.f, -65.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_back-background2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3200, 1800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-800.f, 150.f, -40.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_back-background4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3200, 1800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(700.f, 150.f, -48.f));
	}

	//Background 1
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		mushbg1 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 300.f, -50.f));
	}

	//Background 2
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		mushbg2 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 300.f, -50.f));
	}

	//Background 3
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		mushbg3 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 300.f, -50.f));
	}

	//Background 4
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		mushbg4 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 300.f, -50.f));
	}

	//Background 5
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		mushbg5 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background5.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 300.f, -50.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "fadeIn.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2560, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-400.f, 300.f, -49.f));
	}


	////Setup new Boss
	//{
	//	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//	//Creates entity
	//	auto entity = ECS::CreateEntity();

	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);


	//	//Set up the components
	//	std::string fileName = "testgolem.png";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 173, 130);
	//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 100.f, 0.02f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(0), float32(0));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.f, 69.f);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	tempPhsBody.SetRotationAngleDeg(0);
	//	
	//}

	////Setup new Entity
	//{
	//	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//	//Creates entity
	//	auto entity = ECS::CreateEntity();
	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//Set up the components
	//	std::string fileName = "BeachBall.png";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
	//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.02f));
	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_dynamicBody;
	//	tempDef.position.Set(float32(400), float32(500));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, ENEMY, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.f, 0.f);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	tempPhsBody.SetRotationAngleDeg(0);
	//	tempPhsBody.SetName("Bat");
	//	tempPhsBody.SetHealth(2);

	//}

	//Setup trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		attackTrigger1 = entity;
		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<Sprite>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 56, 75);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 0.002f));
		ECS::GetComponent<Trigger*>(entity) = new AttackTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(MainEntities::MainPlayer());
		//AttackTrigger* temp = (AttackTrigger*)ECS::GetComponent<Trigger*>(entity);
		//temp->movement = b2Vec2(0.f, 15.f);

		//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(mushroomBoss);
		//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(mushroomBoss2);
		//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(dummy);



		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(80.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		//Size of body doesnt match sprite. Reference the other trigger to see how to set up.
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, ENEMY);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetName("Trigger");
	}

	//Setup trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		translateTrigger1 = entity;
		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<Sprite>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 400, 50);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 0.002f));
		ECS::GetComponent<Trigger*>(entity) = new TranslateTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		
		TranslateTrigger* temp = (TranslateTrigger*)ECS::GetComponent<Trigger*>(entity);
		temp->movement = b2Vec2(642.f, 322.f);

		//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(mushroomBoss);
		//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(mushroomBoss2);
		//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(dummy);



		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(910.f), float32(25.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetName("Trigger");
	}

	//Setup trigger back to the Village
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		
		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<Sprite>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 0.002f));
		ECS::GetComponent<Trigger*>(entity) = new TransitionTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

		TransitionTrigger* temp = (TransitionTrigger*)ECS::GetComponent<Trigger*>(entity);
		temp->nextScene = 0;

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-900.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetName("Trigger");
	}

	//Setup trigger to boss room
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<Sprite>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f,0.f, 0.002f));
		ECS::GetComponent<Trigger*>(entity) = new TransitionTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

		TransitionTrigger* temp = (TransitionTrigger*)ECS::GetComponent<Trigger*>(entity);
		temp->nextScene = 2;

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1965.f), float32(500.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetName("Trigger");
	}



	/*//Jump Boost Collect Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		jumpBoostEnitity = entity;
		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<Sprite>(entity);

		//Sets up components
		std::string fileName = "jumpBoost/Boost1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.0f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(160.f, 80.f, 0.5f));
		ECS::GetComponent<Trigger*>(entity) = new JumpPowerupTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);


		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(160.f), float32(80.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		//Size of body doesnt match sprite. Reference the other trigger to see how to set up.
		tempPhsBody = PhysicsBody(entity, tempBody, float(40.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetName("JumpBoostTrigger");
	}*/

	//Some Old Startup Stuff
	resetGrid();
	readSaveFile();
	makeLoadingScreen();

	for (int y = 0 + 25; y < (gWidth * 50) + 25; y += 50)
	{
		for (int x = 0 - 920; x < (gLength * 50) - 920; x += 50)
		{
			RayCastCallback nodeRay;
			m_physicsWorld->RayCast(&nodeRay, b2Vec2(x, y) + b2Vec2(0, 50), b2Vec2(x, y));
			if (nodeRay.m_fixture == nullptr)
			{
				m_physicsWorld->RayCast(&nodeRay, b2Vec2(x, y) + b2Vec2(0, -50), b2Vec2(x, y));
				if (nodeRay.m_fixture == nullptr)
				{
					m_physicsWorld->RayCast(&nodeRay, b2Vec2(x, y) + b2Vec2(-50, 0), b2Vec2(x, y));
					if (nodeRay.m_fixture == nullptr)
					{
						m_physicsWorld->RayCast(&nodeRay, b2Vec2(x, y) + b2Vec2(50, 0), b2Vec2(x, y));


					}

				}

			}

			//Hmm so for some reason when I add this code to make sure that a node still appears at the AI's spawn point,the game crashes when I get there.
			//if (nodeRay.m_fixture == nullptr || nodeRay.m_fixture->GetBody()->GetType() == b2_dynamicBody)
			//{

			if (nodeRay.m_fixture == nullptr || ECS::GetComponent<PhysicsBody>((int)nodeRay.m_fixture->GetBody()->GetUserData()).GetName() == "Decor" || ECS::GetComponent<PhysicsBody>((int)nodeRay.m_fixture->GetBody()->GetUserData()).GetName() == "Bat" || ECS::GetComponent<PhysicsBody>((int)nodeRay.m_fixture->GetBody()->GetUserData()).GetName() == "Player" || ECS::GetComponent<PhysicsBody>((int)nodeRay.m_fixture->GetBody()->GetUserData()).GetName() == "Boss")
			{
					//makeNode(x, y ,0.3);
					//Adds a blank node to the list
					makeGrid(b2Vec2(x, y));
				

			}


		}
	}
	updateNbors(m_physicsWorld);
	//std::cout << "\nNum:" << mushroomBoss;
	//std::cout << "\nNum2:" << mushroomBoss2;
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	player.theAttackTrigger = attackTrigger1;


	makeBat(-335, 200, 0.02, 0, 10, 10);

	makeBat(-745, 410, 0.02, 0, 10, 10);
	makeBat(-735, 410, 0.02, 0, 10, 10);



	startup = true;
	transitionStarted = false;
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	
}

void PhysicsPlayground::makeBox(float xPos, float yPos, float zPos, float rotation,float width, float height) //add z var
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "greyBox.jpg";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));
	ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON,1.f,69.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
	//tempPhsBody.GetBody()->SetAwake(false);
	

}

void PhysicsPlayground::makeBox2(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "boxSprite.jpg";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);


}

void PhysicsPlayground::makeDummy(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//Creates entity
	auto entity = ECS::CreateEntity();
	dummy = entity;
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Set up the components
	std::string fileName = "LinkStandby.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);

}

void PhysicsPlayground::makeMushroom(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//Creates entity
	auto entity = ECS::CreateEntity();
	
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Set up the components
	std::string fileName = "BackgroundMush.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 40.f;
	float shrinkY = 40.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, OBJECTS, 0.f, 0.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
	tempPhsBody.SetName("Decor");
}

void PhysicsPlayground::makeStalagmite1(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//Creates entity
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Set up the components
	std::string fileName = "Rock_Foreground_-_1.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 40.f;
	float shrinkY = 40.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, OBJECTS, 0.f, 0.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
	tempPhsBody.SetName("Decor");

}

void PhysicsPlayground::makeStalagmite2(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//Creates entity
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Set up the components
	std::string fileName = "Rock_Foreground_-_2.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 40.f;
	float shrinkY = 40.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, OBJECTS, 0.f, 0.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
	tempPhsBody.SetName("Decor");

}

void PhysicsPlayground::makeNode(float xPos, float yPos,float transparency)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

	//Sets up components
	std::string fileName = "LevelEditorUI/open.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(xPos, yPos, 0.04f));
	ECS::GetComponent<Sprite>(entity).SetTransparency(transparency);
}

void PhysicsPlayground::makeBat(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	//Creates entity
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Set up the components
	std::string fileName = "BeachBall.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));
	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, ENEMY, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.f, 0.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
	tempPhsBody.SetName("Bat");
	tempPhsBody.SetHealth(2);
	

}


void PhysicsPlayground::makeIceWall(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Sets up components
	std::string fileName = "iceWall.jpg";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, width, height);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, zPos));

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_staticBody;
	tempDef.position.Set(float32(xPos), float32(yPos));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 0.f, 69.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
	tempPhsBody.SetName("Ice Wall");

	
}


void PhysicsPlayground::writeAutoSaveFile(int file)
{
	char x;
	unsigned int entityNum = 0;
	std::ofstream editorSaveFile;
	bool firstLineDone = false;

	if (file == 1)
	{
		editorSaveFile.open("assets/EditorSaves/MushroomCave/backupSaves/BackupSaveFile1.txt");
	}
	else if (file == 2)
	{
		editorSaveFile.open("assets/EditorSaves/MushroomCave/backupSaves/BackupSaveFile2.txt");
	}
	else if (file == 3)
	{
		editorSaveFile.open("assets/EditorSaves/MushroomCave/backupSaves/BackupSaveFile3.txt");
	}
	else if (file == 4)
	{
		editorSaveFile.open("assets/EditorSaves/MushroomCave/backupSaves/BackupSaveFile4.txt");
	}
	else if (file == 5)
	{
		editorSaveFile.open("assets/EditorSaves/MushroomCave/backupSaves/BackupSaveFile5.txt");
	}

	b2Body* body = m_physicsWorld->GetBodyList();


	for (int f = 0; f < m_physicsWorld->GetBodyCount(); f++)
	{
		if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetBody()->GetType() == b2_staticBody && ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetName() != "Trigger")
		{
			if (firstLineDone == true)
			{
				editorSaveFile << "\n" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x;
			}
			else
			{
				editorSaveFile << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x;
				firstLineDone = true;
			}
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().y;
			editorSaveFile << "\t" << ECS::GetComponent<Transform>((int)body->GetUserData()).GetPosition().z;
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetRotationAngleDeg();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetWidth();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetHeight();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetFileName();


		}



		body = body->GetNext();
	}

	editorSaveFile.close();
}

void PhysicsPlayground::writeSaveFile()
{
	char x;
	unsigned int entityNum = 0;
	std::ofstream editorSaveFile;
	editorSaveFile.open("assets/EditorSaves/MushroomCave/LevelEditorSave.txt");
	b2Body* body = m_physicsWorld->GetBodyList();
	bool firstLineDone = false;

	for (int f = 0; f < m_physicsWorld->GetBodyCount(); f++)
	{
		if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetBody()->GetType() == b2_staticBody && ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetName() != "Trigger")
		{
			if (firstLineDone == true)
			{
				editorSaveFile << "\n" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x;
			}
			else
			{
				editorSaveFile << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x;
				firstLineDone = true;
			}
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().y;
			editorSaveFile << "\t" << ECS::GetComponent<Transform>((int)body->GetUserData()).GetPosition().z;
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetRotationAngleDeg();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetWidth();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetHeight();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetFileName();
		}



		body = body->GetNext();
	}
	
	editorSaveFile.close();
}

void PhysicsPlayground::readSaveFile()
{
	float xPos, yPos,zPos, angle, width, height;
	std::string name;
	unsigned int entityNum = 0;
	std::fstream editorSaveFile;

	editorSaveFile.open("assets/EditorSaves/MushroomCave/LevelEditorSave.txt");
	while (editorSaveFile.good())
	{

			editorSaveFile >> xPos;
			editorSaveFile >> yPos;
			editorSaveFile >> zPos;
			editorSaveFile >> angle;
			editorSaveFile >> width;
			editorSaveFile >> height;
			editorSaveFile >> name;

			if (name == "greyBox.jpg")
			{
				makeBox(xPos, yPos,zPos, angle, width, height);
			}
			else if (name == "boxSprite.jpg")
			{
				makeBox2(xPos, yPos, zPos, angle, width, height);
			}
			else if (name == "LinkStandby.png")
			{
				makeDummy(xPos, yPos, zPos, angle, width, height);
			}
			else if (name == "BackgroundMush.png")
			{
				makeMushroom(xPos, yPos, zPos, angle, width, height);
			}
			else if (name == "iceWall.jpg")
			{
				makeIceWall(xPos, yPos, zPos, angle, width, height);
			}
			else if (name == "Rock_Foreground_-_1.png")
			{
				makeStalagmite1(xPos, yPos, zPos, angle, width, height);
			}
			else if (name == "Rock_Foreground_-_2.png")
			{
				makeStalagmite2(xPos, yPos, zPos, angle, width, height);
			}

	}
	editorSaveFile.close();

	if (startup == true)
	{
		//std::fstream playerSaveFile;
		//playerSaveFile.open("assets/PlayerSaves/File1.txt");

		//float newXPos, newYPos;
		//playerSaveFile >> newXPos;
		//playerSaveFile >> newYPos;

		//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(b2Vec2(newXPos, newYPos));
		//ECS::GetComponent<PhysicsBody>(playerFollow).SetPosition(b2Vec2(newXPos, newYPos));

		//playerSaveFile.close();
	}
}


void PhysicsPlayground::animateBackground()
{
	bgtimer += 1 * Timer::deltaTime;
	std::string fileName;
	if (bgtimer > 1.25) {
		bgtimer = 0;
	}

	if (bgtimer > 1)
	{
		ECS::GetComponent<Sprite>(mushbg5).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(mushbg4).SetTransparency(0.f);
	}
	else if (bgtimer > 0.75)
	{
		ECS::GetComponent<Sprite>(mushbg4).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(mushbg3).SetTransparency(0.f);

	}
	else if (bgtimer > 0.5)
	{
		ECS::GetComponent<Sprite>(mushbg3).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(mushbg2).SetTransparency(0.f);

	}
	else if (bgtimer > 0.25)
	{
		ECS::GetComponent<Sprite>(mushbg2).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(mushbg1).SetTransparency(0.f);

	}
	else if (bgtimer < 0.25 )
	{
		ECS::GetComponent<Sprite>(mushbg1).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(mushbg5).SetTransparency(0.f);

	}
	//std::cout << "\n" << bgtimer;
	

}

void PhysicsPlayground::loadNear()
{
	
		float xPos, yPos, zPos, angle, width, height;
		std::string name;
		unsigned int entityNum = 0;
		std::fstream editorSaveFile;
		bool alreadyCreated = false;



		editorSaveFile.open("assets/EditorSaves/MushroomCave/LevelEditorSave.txt");
		while (editorSaveFile.good())
		{


			alreadyCreated = false;
			editorSaveFile >> xPos;
			editorSaveFile >> yPos;
			editorSaveFile >> zPos;
			editorSaveFile >> angle;
			editorSaveFile >> width;
			editorSaveFile >> height;
			editorSaveFile >> name;

			b2Body* body = m_physicsWorld->GetBodyList();
			for (int f = 0; f < m_physicsWorld->GetBodyCount(); f++)
			{

				if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetBody()->GetType() == b2_staticBody && ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetName() != "Trigger")
				{
					if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x == xPos && ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().y == yPos)
					{

						alreadyCreated = true;
					}
				}
				body = body->GetNext();
			}
			
			if (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x + 500 > xPos - (width/2) && ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - 500 < xPos + (width / 2))
			{
				if (alreadyCreated == false)
				{
					std::cout << "\n" << m_physicsWorld->GetBodyCount();

					if (name == "greyBox.jpg")
					{
						makeBox(xPos, yPos, zPos, angle, width, height);
					}
					else if (name == "boxSprite.jpg")
					{
						makeBox2(xPos, yPos, zPos, angle, width, height);
					}
					else if (name == "LinkStandby.png")
					{
						makeDummy(xPos, yPos, zPos, angle, width, height);
					}
					else if (name == "BackgroundMush.png")
					{
						makeMushroom(xPos, yPos, zPos, angle, width, height);
					}
					else if (name == "iceWall.jpg")
					{
						makeIceWall(xPos, yPos, zPos, angle, width, height);
					}
					else if (name == "Rock_Foreground_-_1.png")
					{
						makeStalagmite1(xPos, yPos, zPos, angle, width, height);
					}
					else if (name == "Rock_Foreground_-_2.png")
					{
						makeStalagmite2(xPos, yPos, zPos, angle, width, height);
					}
				}

			}
			
			else
			{

				b2Body* body = m_physicsWorld->GetBodyList();
				for (int f = 0; f < m_physicsWorld->GetBodyCount(); f++)
				{
					
					if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetBody()->GetType() == b2_staticBody && ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetName() != "Trigger")
					{
						if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x == xPos && ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().y == yPos)
						{
							//std::cout << "\n" << (int)body->GetUserData();
							PhysicsBody::m_bodiesToDelete.push_back((int)body->GetUserData());
						}
					}
					body = body->GetNext();
				}
				
			}
			

		}
		editorSaveFile.close();
}

void PhysicsPlayground::cameraTrackPlayer()
{

	b2Vec2 newPos = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition() + b2Vec2(mousePosX / 30, (mousePosY / 15) - 10) - ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition();
	float length = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);
	newPos = b2Vec2(newPos.x, newPos.y);

	b2Vec2 speed = b2Vec2(10, 10);
	ECS::GetComponent<PhysicsBody>(playerFollow).GetBody()->SetLinearVelocity(b2Vec2(newPos.x * speed.x, newPos.y * speed.y));

}



void PhysicsPlayground::ZoomCamera()
{

}


void PhysicsPlayground::updateUI()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	if (round(lastDash) != round(airDashCounter)) {
		if (airDashCounter >= 3) { //full dash bar
			std::string fileName = "ui/Dash3.png";
			player.canDash = true;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 22, 5);
		}
		else if (airDashCounter >= 2) {
			std::string fileName = "ui/Dash2.png";
			player.canDash = true;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 22, 5);
		}
		else if (airDashCounter >= 1) {
			std::string fileName = "ui/Dash1.png";
			player.canDash = true;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 22, 5);
		}
		else { //empty dash bar
			std::string fileName = "ui/Dash0.png";
			player.canDash = false;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 22, 5);
		}
		lastDash = airDashCounter;
		dashTrans = 2.f;
		//std::cout << std::endl << airDashCounter << std::endl;
	}

	if (lastHealth != health) {
		if (health >= 6) { //full health bar
			std::string fileName = "ui/Health6.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 22, 5);
		}
		else if (health >= 5) {
			std::string fileName = "ui/Health5.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 22, 5);
		}
		else if (health >= 4) {
			std::string fileName = "ui/Health4.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 22, 5);
		}
		else if (health >= 3) {
			std::string fileName = "ui/Health3.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 22, 5);
		}
		else if (health >= 2) {
			std::string fileName = "ui/Health2.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 22, 5);
		}
		else if (health >= 1) {
			std::string fileName = "ui/Health1.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 22, 5);
		}
		else { //empty health bar
			std::string fileName = "ui/Health0.png";
			player.m_dead = true;
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 22, 5);
		}
		invincibilityTime = invincibilityTimeDefault;
		lastHealth = health;
		hpTrans = 2.f;
	}

	if (hpTrans > 0.f)
	{
		hpTrans -= 1.f * Timer::deltaTime;

	}

	if (dashTrans > 0.f)
	{
		dashTrans -= 1.f * Timer::deltaTime;
	}

	if (invincibilityTime > 0)
	{
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetTransparency(0.5f);
		invincibilityTime -= 1.f * Timer::deltaTime;
	}
	else
	{
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetTransparency(1.f);
		invincibilityTime = 0;

		if (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetContactList() != NULL)
		{
			int currentEnemy = (int)ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData();

			if (ECS::GetComponent<PhysicsBody>(currentEnemy).GetName() == "Bat" && startAttackCooldown == false)
			{
				health--;
				b2Vec2 towardPlayer = ECS::GetComponent<PhysicsBody>(currentEnemy).CalculateMovement(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(towardPlayer.x * 200000, towardPlayer.y * 200000), true); //Knocks back the player, but gets immediately canceled by player movement
				startAttackCooldown = true;
			}

			if (ECS::GetComponent<PhysicsBody>(currentEnemy).GetName() == "Boss" && startAttackCooldown == false)
			{

				b2Vec2 towardPlayer;
				towardPlayer = ECS::GetComponent<PhysicsBody>(currentEnemy).CalculateMovement(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());

				if (ECS::GetComponent<PhysicsBody>(currentEnemy).isCharging == true)
				{
					ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(towardPlayer.x * 1000000, 20000000.f), true);
					health -= 3;
				}
				else if (ECS::GetComponent<PhysicsBody>(currentEnemy).isAttacking == true)
				{
					ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(towardPlayer.x * 1000000, towardPlayer.y * 1000000), true);
					health -= 2;


				}
				else
				{
					ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(towardPlayer.x * 1000000, towardPlayer.y * 1000000), true);
					health--;

				}
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(towardPlayer.x * 1000000, towardPlayer.y * 1000000), true); //Knocks back the player, but gets immediately canceled by player movement
				startAttackCooldown = true;
			}


		}

		if (startAttackCooldown == true)
		{
			attackCooldownTimer -= 1 * Timer::deltaTime;
			if (attackCooldownTimer <= 0)
			{
				attackCooldownTimer = attackCooldownTimerDefault;
				startAttackCooldown = false;

			}
		}

	}

	ECS::GetComponent<Sprite>(healthBar).SetTransparency(hpTrans);
	ECS::GetComponent<Sprite>(dashBar).SetTransparency(dashTrans);

	auto& playerBody = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	ECS::GetComponent<Transform>(dashBar).SetPosition(vec3(playerBody.GetPosition().x, playerBody.GetPosition().y, 0.01) + vec3(0, 25, 0.01f));
	ECS::GetComponent<Transform>(healthBar).SetPosition(vec3(playerBody.GetPosition().x, playerBody.GetPosition().y, 0.01) + vec3(0, 33, 0.01f));


}

void PhysicsPlayground::SavePlayerLoc() {
	std::ofstream fstre;
	std::string fileLoc = "assets/PlayerSaves/File2.txt";

	int tempx = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x;
	int tempy = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y;

	fstre.open(fileLoc);
	if (!fstre) {
		std::cout << "Failed to save player location!" << std::endl;
	}
	else {
		fstre << tempx << " " << tempy;
	}
}

b2Vec2 PhysicsPlayground::LoadPlayerLoc() {
	std::fstream fstre;
	std::string fileLoc = "assets/PlayerSaves/File2.txt";
	int tempx;
	int tempy;

	fstre.open(fileLoc);
	if (!fstre) {
		std::cout << "Failed to load player location!" << std::endl;
	}
	else {
		fstre >> tempx >> tempy;
	}

	return b2Vec2(tempx, tempy);
}

void PhysicsPlayground::Update()
{

	animateBackground();

	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto pl = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	player.Update();
	player.canYouFuckingJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).m_canJump;
	player.haveYouPressedSpace = spacePressed;
	if (player.m_attacking && player.haveYouPressedSpace == false) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(256);
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetHeight(128);
	}
	else if (player.m_dashing) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(448);
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetHeight(64);
	}
	else if (player.m_dead) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(96);
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetHeight(64);
	}
	else
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(64);
		if (!player.m_attacking) ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetHeight(64);


	if (pl.GetPosition().x > 130 && pl.GetPosition().x  < 190 && pl.GetPosition().y < 100) {
		hasChargeJump = true;
		ECS::GetComponent<Sprite>(jumpBoostEntity).SetTransparency(0.f);
	}

	diffTime3 = (clock() - startTime3) / (double)(CLOCKS_PER_SEC);
	diffTimeJumpBoost = (clock() - startTimeJumpBoost) / (double)(CLOCKS_PER_SEC);

	puni2DiffTime = (clock() - puni2StartTime) / (double)(CLOCKS_PER_SEC);

	if (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x > -750 && !activatePuni2) {
		activatePuni2 = true;
		puni2StartTime = clock();
		health = 6;
		ECS::GetComponent<Sprite>(puni2Text).SetTransparency(1.f);
		SavePlayerLoc();
	}

	if (activatePuni2 && puni2DiffTime > 0.12) {
		std::string fileName = "punisher/Punisher-Cooking1.png";
		puni2StartTime = clock();
		puni2CurFrame += 1;

		switch (puni2CurFrame) {
		case 1:
			fileName = "punisher/Punisher-Cooking1.png";
			break;
		case 2:
			fileName = "punisher/Punisher-Cooking2.png";
			break;
		case 3:
			fileName = "punisher/Punisher-Cooking3.png";
			break;
		case 4:
			fileName = "punisher/Punisher-Cooking4.png";
			break;
		case 5:
			fileName = "punisher/Punisher-Cooking5.png";
			break;
		case 6:
			fileName = "punisher/Punisher-Cooking6.png";
			break;
		case 7:
			fileName = "punisher/Punisher-Cooking7.png";
			break;
		case 8:
			fileName = "punisher/Punisher-Cooking8.png";
			break;
		case 9:
			fileName = "punisher/Punisher-Cooking9.png";
			break;
		case 10:
			fileName = "punisher/Punisher-Cooking10.png";
			break;
		case 11:
			fileName = "punisher/Punisher-Cooking11.png";
			break;
		case 12:
			fileName = "punisher/Punisher-Cooking12.png";
			break;
		case 13:
			fileName = "punisher/Punisher-Cooking13.png";
			break;
		case 14:
			fileName = "punisher/Punisher-Cooking14.png";
			break;
		case 15:
			fileName = "punisher/Punisher-Cooking15.png";
			break;
		case 16:
			fileName = "punisher/Punisher-Cooking16.png";
			break;
		case 17:
			fileName = "punisher/Punisher-Cooking17.png";
			break;
		case 18:
			fileName = "punisher/Punisher-Cooking18.png";
			break;
		case 19:
			fileName = "punisher/Punisher-Cooking19.png";
			break;
		case 20:
			fileName = "punisher/Punisher-Cooking20.png";
			break;
		case 21:
			fileName = "punisher/Punisher-Cooking21.png";
			break;
		case 22:
			fileName = "punisher/Punisher-Cooking22.png";
			break;
		case 23:
			fileName = "punisher/Punisher-Cooking23.png";
			break;
		case 24:
			fileName = "punisher/Punisher-Cooking24.png";
			break;
		case 25:
			fileName = "punisher/Punisher-Cooking25.png";
			break;
		default:
			fileName = "punisher/Punisher-Cooking26.png";
			break;
		}

		ECS::GetComponent<Sprite>(puni2).LoadSprite(fileName, 128, 128);
	}

	//std::cout << diffTimeJumpBoost << "   " << jumpBoostFrame << std::endl;

	if (health <= 0) {
		health = 6;
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(LoadPlayerLoc());
	}

	if (diffTimeJumpBoost > 0.2) {

		std::string fileName = "jumpBoost/boost1.png";

		startTimeJumpBoost = clock();

		jumpBoostFrame = jumpBoostFrame + 1;

		switch (jumpBoostFrame) {
		case 1:
			fileName = "jumpBoost/boost1.png";
			break;
		case 2:
			fileName = "jumpBoost/boost2.png";
			break;
		case 3:
			fileName = "jumpBoost/boost3.png";
			break;
		case 4:
			fileName = "jumpBoost/boost4.png";
			break;
		case 5:
			fileName = "jumpBoost/boost5.png";
			break;
		case 6:
			fileName = "jumpBoost/boost6.png";
			break;
		case 7:
			fileName = "jumpBoost/boost7.png";
			break;
		default:
			fileName = "jumpBoost/boost8.png";
			jumpBoostFrame = 0;
			break;
		}

		ECS::GetComponent<Sprite>(jumpBoostEntity).LoadSprite(fileName, 32, 32);

	}

	

	if (startup == false)
	{
		startup = true;
		
	}


	cameraTrackPlayer();
	ZoomCamera();
	updateUI();
	CheckTransition();
	//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetGravityScale(-m_gravity.y * Timer::deltaTime);
	//std::cout << "\n" << airDashCounter;
	if (levelEditor == false)
	{
		levelEditorStartup = false;

		if (Timer::currentClock % 60 == 0)
		{
			loadNear();

		}
	}
	else
	{
		if (levelEditorStartup == false)
		{

			b2Body* body = m_physicsWorld->GetBodyList();
			for (int f = 0; f < m_physicsWorld->GetBodyCount(); f++)
			{

				if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetBody()->GetType() == b2_staticBody && ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetName() != "Trigger")
				{
						PhysicsBody::m_bodiesToDelete.push_back((int)body->GetUserData());
				}
				body = body->GetNext();
			}
			readSaveFile();
			levelEditorStartup = true;
		}
		
	}
	
}

void PhysicsPlayground::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}
}

void PhysicsPlayground::GUIWindowUI()
{
	ImGui::Begin("Test");

	ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine();
	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
	{
		m_secondWindow = !m_secondWindow;
	}
	
	ImGui::End();
}

void PhysicsPlayground::GUIWindowOne()
{
	//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();
	
	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		ECS::GetComponent<PhysicsBody>(puzzleWall1).SetPosition(b2Vec2(gravity[0], gravity[1]));
		//m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();
	
	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();
}

void PhysicsPlayground::GUIWindowTwo()
{
	//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0,1), ImVec2(1,0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();

	ImGui::End();
}

void PhysicsPlayground::RunLevelEditor()
{
	float scaleSpeed = 10;
	b2Vec2 wMousePos;
	wMousePos = (b2Vec2(mousePosX / 5, mousePosY / 5));
	wMousePos += b2Vec2(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition().x, ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition().y);
	ECS::GetComponent<Sprite>(rayMarker).SetTransparency(0.8f);
	ECS::GetComponent<Transform>(rayMarker).SetPosition(wMousePos.x, wMousePos.y, 2);

	ECS::GetComponent<Sprite>(editorEnabled).SetTransparency(0.8f);
	ECS::GetComponent<Transform>(editorEnabled).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition() + vec3(-180, 130, 2));

	ECS::GetComponent<Sprite>(changesSaved).SetTransparency(0.f);
	ECS::GetComponent<Transform>(changesSaved).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition() + vec3(-95, -60, 2));
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset(0.f);
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(editZone));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(0.f);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(editZone));

	if (entitiesCreated == false)
	{
		RayCastCallback selectRay;
		m_physicsWorld->RayCast(&selectRay, wMousePos + b2Vec2(0, 20), wMousePos);
		if (selectRay.m_fixture == nullptr)
		{
			m_physicsWorld->RayCast(&selectRay, wMousePos + b2Vec2(0, -20), wMousePos);
			if (selectRay.m_fixture == nullptr)
			{
				m_physicsWorld->RayCast(&selectRay, wMousePos + b2Vec2(-20, 0), wMousePos);
				if (selectRay.m_fixture == nullptr)
				{
					m_physicsWorld->RayCast(&selectRay, wMousePos + b2Vec2(20, 0), wMousePos);


				}

			}

		}

		if (selectRay.m_fixture != nullptr)
		{
			std::string fileName = "LevelEditorUI/Cursor.png";
			ECS::GetComponent<Sprite>(rayMarker).LoadSprite(fileName, 10, 10);

		}
		else
		{
			std::string fileName = "LevelEditorUI/Cursor2.png";
			ECS::GetComponent<Sprite>(rayMarker).LoadSprite(fileName, 10, 10);
		}

		if (mouseEvnt.button == 1 && mouseEvnt.state == SDL_PRESSED)
		{

			if (selectRay.m_fixture != nullptr)
			{
				if (objectAlreadySelected == false)
				{
					//writeUndoFile(m_physicsWorld);
					std::cout << "\nX: " << selectRay.m_fixture->GetBody()->GetPosition().x << " Y: " << selectRay.m_fixture->GetBody()->GetPosition().y;
					selectedEntity = (int)selectRay.m_fixture->GetBody()->GetUserData();
					objectAlreadySelected = true;
				}
			}
			if (selectedEntity != NULL)
			{
				ECS::GetComponent<PhysicsBody>(selectedEntity).SetPosition(wMousePos);
			}
		}
	}
	else
	{
		entitiesCreated = false;

	}
	if (mouseEvnt.button == 1 && mouseEvnt.state == SDL_RELEASED)
	{
		objectAlreadySelected = false;
		selectedEntity = NULL;
	}

	if (Input::GetKey(Key::Q))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<PhysicsBody>(selectedEntity).SetRotationAngleDeg(ECS::GetComponent<PhysicsBody>(selectedEntity).GetRotationAngleDeg() + (60.f * Timer::deltaTime));
		}
	}

	if (Input::GetKey(Key::E))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<PhysicsBody>(selectedEntity).SetRotationAngleDeg(ECS::GetComponent<PhysicsBody>(selectedEntity).GetRotationAngleDeg() - (60.f * Timer::deltaTime));
		}
	}


	if (Input::GetKey(Key::R))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<Transform>(selectedEntity).SetPositionZ(ECS::GetComponent<Transform>(selectedEntity).GetPositionZ() - 2 * Timer::deltaTime);
		}
	}

	if (Input::GetKey(Key::F))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<Transform>(selectedEntity).SetPositionZ(ECS::GetComponent<Transform>(selectedEntity).GetPositionZ() + 2 * Timer::deltaTime);
		}
	}

	float editorCamSpeed = 80.f;
	if (Input::GetKey(Key::Shift))
	{
		editorCamSpeed *= 2;
		scaleSpeed *= 3;
	}

	if (Input::GetKey(Key::W))
	{
		ECS::GetComponent<Transform>(editZone).SetPosition(ECS::GetComponent<Transform>(editZone).GetPosition() + vec3(0, editorCamSpeed * Timer::deltaTime, 0));
	}
	if (Input::GetKey(Key::S))
	{
		ECS::GetComponent<Transform>(editZone).SetPosition(ECS::GetComponent<Transform>(editZone).GetPosition() + vec3(0, -editorCamSpeed * Timer::deltaTime, 0));
	}
	if (Input::GetKey(Key::D))
	{
		ECS::GetComponent<Transform>(editZone).SetPosition(ECS::GetComponent<Transform>(editZone).GetPosition() + vec3(editorCamSpeed * Timer::deltaTime, 0, 0));
	}
	if (Input::GetKey(Key::A))
	{
		ECS::GetComponent<Transform>(editZone).SetPosition(ECS::GetComponent<Transform>(editZone).GetPosition() + vec3(-editorCamSpeed * Timer::deltaTime, 0, 0));
	}

	if (Input::GetKeyDown(Key::H))
	{

		ECS::GetComponent<Sprite>(changesSaved).SetTransparency(1.f);
		saveUITimer = 2.f;
		writeSaveFile();
		std::cout << "\nChanges Saved.";

	}

	if (saveUITimer > 0)
	{

		saveUITimer -= 1 * Timer::deltaTime;
		ECS::GetComponent<Sprite>(changesSaved).SetTransparency(saveUITimer);
	}

	if (Input::GetKeyDown(Key::Z))
	{
		//readUndoFile(m_physicsWorld);

	}

	if (Input::GetKeyDown(Key::One))
	{
		entitiesCreated = true;
		makeBox2(wMousePos.x, wMousePos.y, 0.03f, 0, 128, 128);

	}
	else if (Input::GetKeyDown(Key::Two))
	{
		entitiesCreated = true;
		makeBox2(wMousePos.x, wMousePos.y, 0.03f, 0, 64, 64);

	}
	else if (Input::GetKeyDown(Key::Three))
	{
		entitiesCreated = true;
		makeDummy(wMousePos.x, wMousePos.y, 0.02f, 0, 50, 50);

	}
	else if (Input::GetKeyDown(Key::Four))
	{
		entitiesCreated = true;
		makeMushroom(wMousePos.x, wMousePos.y, 0.05f, 0, 73, 99);

	}
	else if (Input::GetKeyDown(Key::Five))
	{
		entitiesCreated = true;
		makeBat(wMousePos.x, wMousePos.y, 0.02f, 0, 10, 10);

	}

	else if (Input::GetKeyDown(Key::Six))
	{
		entitiesCreated = true;
		makeIceWall(wMousePos.x, wMousePos.y, 0.02f, 0, 50, 50);

	}
	else if (Input::GetKeyDown(Key::Seven))
	{
		entitiesCreated = true;
		makeStalagmite1(wMousePos.x, wMousePos.y, 0.05f, 0, 128, 128);

	}
	else if (Input::GetKeyDown(Key::Eight))
	{
		entitiesCreated = true;
		makeStalagmite2(wMousePos.x, wMousePos.y, 0.05f, 0, 128, 128);

	}
	if (Input::GetKey(Key::RightArrow))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<PhysicsBody>(selectedEntity).ScaleBodyWidth(scaleSpeed * Timer::deltaTime, 0);
		}
	}
	else if (Input::GetKey(Key::LeftArrow))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<PhysicsBody>(selectedEntity).ScaleBodyWidth(-scaleSpeed * Timer::deltaTime, 0);
		}
	}

	if (Input::GetKey(Key::UpArrow))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<PhysicsBody>(selectedEntity).ScaleBodyHeight(scaleSpeed * Timer::deltaTime, 0);
		}
	}
	else if (Input::GetKey(Key::DownArrow))
	{
		if (selectedEntity != NULL)
		{
			ECS::GetComponent<PhysicsBody>(selectedEntity).ScaleBodyHeight(-scaleSpeed * Timer::deltaTime, 0);
		}
	}
	if (Input::GetKeyDown(Key::Backspace))
	{
		if (selectedEntity != NULL)
		{
			PhysicsBody::m_bodiesToDelete.push_back(selectedEntity);
			selectedEntity = NULL;
		}
	}
	if (selectedEntity == NULL)
	{
		if (Input::GetKey(Key::UpArrow))
		{
			fov += 2 * Timer::deltaTime;
			std::cout << "\n" << fov;
			
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());

		}
		else if (Input::GetKey(Key::DownArrow))
		{
			fov -= 2 * Timer::deltaTime;
			std::cout << "\n" << fov;
			
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());

		}

		if (Input::GetKey(Key::RightArrow))
		{
			nPlane += 2 * Timer::deltaTime;
			std::cout << "\n" << nPlane;
			
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());


		}
		else if (Input::GetKey(Key::LeftArrow))
		{
			nPlane -= 2 * Timer::deltaTime;
			std::cout << "\n" << nPlane;
			

			ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());


		}
	}

	//Autosave Cycle
	autosaveTimer += 1.f * Timer::deltaTime;

	if ((int)autosaveTimer == autosaveInterval)
	{
		writeAutoSaveFile(1);
		std::cout << "\nBackup Saved to File 1";
	}
	else if ((int)autosaveTimer == autosaveInterval * 2)
	{
		writeAutoSaveFile(2);
		std::cout << "\nBackup Saved to File 2";

	}
	else if ((int)autosaveTimer == autosaveInterval * 3)
	{
		writeAutoSaveFile(3);
		std::cout << "\nBackup Saved to File 3";

	}
	else if ((int)autosaveTimer == autosaveInterval * 4)
	{
		writeAutoSaveFile(4);
		std::cout << "\nBackup Saved to File 4";

	}
	else if ((int)autosaveTimer >= autosaveInterval * 5)
	{
		writeAutoSaveFile(5);
		std::cout << "\nBackup Saved to File 5";
		autosaveTimer = 0;
	}


}


void PhysicsPlayground::KeyboardHold()
{

	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());


	if (levelEditor == true)
	{
		RunLevelEditor();
	}
	else
	{
		selectedEntity = NULL;
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset(1.f);
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(1.f);

		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(playerFollow));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(playerFollow));
		ECS::GetComponent<Sprite>(rayMarker).SetTransparency(0.f);
		ECS::GetComponent<Sprite>(editorEnabled).SetTransparency(0.f);

		ECS::GetComponent<Transform>(editZone).SetPosition(player.GetPosition().x, player.GetPosition().y, 2);

		PlayerMovement();

	}
	if (Input::GetKeyDown(Key::Tab))
	{
		cameraZoom = cameraZoomDefault;
		vec4 temp = vec4(-cameraZoomDefault, cameraZoomDefault, -cameraZoomDefault, cameraZoomDefault);
		ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetOrthoSize(temp);

		if (levelEditor == false)
		{
			levelEditor = true;
		}
		else
		{
			levelEditor = false;
		}
	}

	if (Input::GetKeyDown(Key::K))
	{
		loadNear();
	}


}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::OEMMinus) && health > 0) health--;
	if (Input::GetKeyDown(Key::OEMPlus) && health < 6) health++;


	if (loadStarted == false) {
		loadStarted = true;
	}

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}

	if (Input::GetKeyDown(Key::J))
	{
		findStartAndEnd(ECS::GetComponent<PhysicsBody>(mushroomBoss).GetPosition(), ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition());
		CalculatePath();

		for (int n = 0; n < getNodeCount(); n++)
		{
			if (getNodeList()[n].isPath())
			{
				makeNode(getNodeList()[n].position.x, getNodeList()[n].position.y, 0.125f * getNodeList()[n].nborsCount);
			}
		}

	}

}

void PhysicsPlayground::KeyboardUp()
{
	

}

void PhysicsPlayground::MouseMotion(SDL_MouseMotionEvent evnt)
{

	mousePosX = evnt.x - (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().x / 2);
	mousePosY = (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y - evnt.y) - (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y / 2);

}

void PhysicsPlayground::MouseClick(SDL_MouseButtonEvent evnt)
{
	mouseEvnt = evnt;

}
