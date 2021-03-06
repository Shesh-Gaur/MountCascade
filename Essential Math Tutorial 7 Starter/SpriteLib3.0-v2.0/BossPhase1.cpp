#include "BossPhase1.h"
#include "Utilities.h"
#include "RayCastCallback.h"
#include <random>
#include <fstream>
#include <iostream>
#include "Astar.h"

BossPhase1::BossPhase1(std::string name)
	: Scene(name)
{

	m_gravity = b2Vec2(0.f, -235.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void BossPhase1::InitScene(float windowWidth, float windowHeight)
{

	m_physicsWorld = new b2World(m_gravity);
	m_name = "Mt.Cascade";
	m_gravity = b2Vec2(0.f, -235.f);
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
		//Add components
		ov1 = entity;
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "overlay/Boss_Stage_1_Overlay.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 4096, 4096);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(315.f, 853.f, 0.021f));
	}

	{
		auto entity = ECS::CreateEntity();
		//Add components
		ov2 = entity;
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "overlay/Boss_Stage_2_Overlay.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 4096, 4096);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(315.f, 853.f, 0.021f));
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
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(200.f, 150.f, -65.f));
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(300.f, 1460.f, -48.f));
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
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-700.f, 200.f, -40.f));
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
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 200.f, -40.f));
	}


	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		//bg1 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Lava_Cave.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 400.f, -50.f));
	}

	//Setup new Entity
	/*{

		//Creates entity
		auto entity = ECS::CreateEntity();
		bg2 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 400.f, -50.f));
	}

	//Setup new Entity
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		bg3 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 400.f, -50.f));
	}

	//Setup new Entity
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		bg4 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background4.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 400.f, -50.f));
	}

	//Setup new Entity
	{

		//Creates entity
		auto entity = ECS::CreateEntity();
		bg5 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Cave_background5.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, 400.f, -50.f));
	}*/

	//Setup new Entity
	{

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "fadeIn2.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2080, 2560);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-100.f, 900.f, -49.f));
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
		std::string fileName = "fadeIn3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2560, 2080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1340.f, 300.f, -49.f));
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 240, 125);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 65, 15);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
	}

	{ //Punisher Text Entity

		auto entity = ECS::CreateEntity();
		puni3Text = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "tutorial/Chatbox-Punisher Healing Station.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 96, 48);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.0f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-420.f, 1300.f, 0.01f));
	}

	{ //Punisher Entity

	//Creates entity
		auto entity = ECS::CreateEntity();
		punisher = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "punisher/Punisher-Cooking1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 128);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.0f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-540.f, 1300.f, 0.01f));
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.02f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0), float32(100));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, OBJECTS, PICKUP, 0.f, 0.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);
		tempPhsBody.SetGravityScale(0.f);

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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 160, 90);
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
		tempDef.position.Set(float32(-724.f), float32 (1268.f));

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
		std::string fileName = "white.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 6200, 1800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(300.f, 150.f, -70.f));
	}


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


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(580.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		//Size of body doesnt match sprite. Reference the other trigger to see how to set up.
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, ENEMY);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetName("Trigger");
	}

	//BOSS HEALTH BAR
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		bossHpBar = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Boss_Bar_Outline.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512, 512);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, 800.f, 0.03f));

	}

	//BOSS HEALTH BAR
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		bossInBar = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "Masks/SquareMask.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 510, 12);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, 552.f, 0.03f));
	}

	//Create BOSS
	{
		auto entity = ECS::CreateEntity();
		boss = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Set up the components
		std::string fileName = "idleframe1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 256, 256);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.02f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 90.f;
		float shrinkY = 145.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(100), float32(350));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		
		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, -72.f), false, ENEMY, PLAYER | ENEMY , 1.f,8.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);
		tempPhsBody.SetHealth(45);
		tempPhsBody.SetMaxHealth(45);
		tempPhsBody.SetSpeed(70.f);
		tempPhsBody.SetHealthBar(bossInBar);
		tempPhsBody.SetName("Boss");



	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		phase1Wall2 = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "greyBox.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 86.f, 545.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.02f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(325.f), float32(422.591f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.f, 69.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);
		tempPhsBody.SetName("Decor");

	}

	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		exitDoor = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "greyBox.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 105.f, 198.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.02f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(976.f), float32(373.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.f, 69.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);
		tempPhsBody.SetName("Decor");

	}

	//Setup trigger to Chass Room
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 90, 50);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.002f));
		ECS::GetComponent<Trigger*>(entity) = new TransitionTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

		TransitionTrigger* temp = (TransitionTrigger*)ECS::GetComponent<Trigger*>(entity);
		temp->nextScene = 3;

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(1667.f), float32(154.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetName("Trigger");
	}



	readSaveFile();
	resetGrid();
	makeLoadingScreen();

	for (int y = 0 + 275; y < (gWidth * 50) + 275; y += 50)
	{
		for (int x = 0 - 275; x < (gLength * 50) - 275; x += 50)
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

				//makeNode(x, y ,1);
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



	//Spawn in platforms when phase 2 starts instead of from save file

	startup = true;
	hasChargeJump = true;
	bossStarted = false;
	fov = 70.f;
	ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
	transitionStarted = false;

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));

}

void BossPhase1::makeBox(float xPos, float yPos, float zPos, float rotation, float width, float height) //add z var
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
	ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
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
		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON, 1.f, 69.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);
	tempPhsBody.SetName("Wall");


}

void BossPhase1::makeBox2(float xPos, float yPos, float zPos, float rotation, float width, float height)
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

void BossPhase1::makeDummy(float xPos, float yPos, float zPos, float rotation, float width, float height)
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

void BossPhase1::makeMushroom(float xPos, float yPos, float zPos, float rotation, float width, float height)
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

void BossPhase1::makeStalagmite1(float xPos, float yPos, float zPos, float rotation, float width, float height)
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

}

void BossPhase1::makeStalagmite2(float xPos, float yPos, float zPos, float rotation, float width, float height)
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

}

void BossPhase1::makeNode(float xPos, float yPos, float transparency)
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

void BossPhase1::makeBat(float xPos, float yPos, float zPos, float rotation, float width, float height)
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

	//batVec2.push_back(entity);

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
	tempPhsBody.SetAgro(true);


}


void BossPhase1::makeIceWall(float xPos, float yPos, float zPos, float rotation, float width, float height)
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
	ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
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

void BossPhase1::makeSmallMush(float xPos, float yPos, float zPos, float rotation, float width, float height)
{
	//Creates entity
	auto entity = ECS::CreateEntity();
	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);

	//Set up the components
	std::string fileName = "smallMush.png";
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

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, OBJECTS, 0.f, 0.f);
	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	tempPhsBody.SetRotationAngleDeg(rotation);


}

void BossPhase1::writeAutoSaveFile(int file)
{
	char x;
	unsigned int entityNum = 0;
	std::ofstream editorSaveFile;
	bool firstLineDone = false;

	if (file == 1)
	{
		editorSaveFile.open("assets/EditorSaves/BossPhase1/backupSaves/BackupSaveFile1.txt");
	}
	else if (file == 2)
	{
		editorSaveFile.open("assets/EditorSaves/BossPhase1/backupSaves/BackupSaveFile2.txt");
	}
	else if (file == 3)
	{
		editorSaveFile.open("assets/EditorSaves/BossPhase1/backupSaves/BackupSaveFile3.txt");
	}
	else if (file == 4)
	{
		editorSaveFile.open("assets/EditorSaves/BossPhase1/backupSaves/BackupSaveFile4.txt");
	}
	else if (file == 5)
	{
		editorSaveFile.open("assets/EditorSaves/BossPhase1/backupSaves/BackupSaveFile5.txt");
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

void BossPhase1::writeSaveFile()
{
	char x;
	unsigned int entityNum = 0;
	std::ofstream editorSaveFile;
	editorSaveFile.open("assets/EditorSaves/BossPhase1/LevelEditorSave.txt");
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

void BossPhase1::readSaveFile()
{
	float xPos, yPos, zPos, angle, width, height;
	std::string name;
	unsigned int entityNum = 0;
	std::fstream editorSaveFile;

	editorSaveFile.open("assets/EditorSaves/BossPhase1/LevelEditorSave.txt");
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
		else if (name == "smallMush.png")
		{
			makeSmallMush(xPos, yPos, zPos, angle, width, height);
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

		//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(b2Vec2(-724.f, 1268.f));
		//ECS::GetComponent<PhysicsBody>(playerFollow).SetPosition(b2Vec2(-724.f, 1268.f));
		
		//playerSaveFile.close();
	}
	ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(b2Vec2(-724.f, 1268.f));
	ECS::GetComponent<PhysicsBody>(playerFollow).SetPosition(b2Vec2(-724.f, 1268.f));

}


void BossPhase1::cameraTrackPlayer()
{
	if (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y < 1100.f && ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y > 800.f)
	{

			bossStarted = true;
		
	}


	if (bossStarted == true && doorDeleted == false)
	{

		if (fov < 83)
		{
			fov += 2.5f * Timer::deltaTime;
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);

		}
		if (screenShakeTimer <= 0)
		{
			if (phase2 == false)
			{
				newPos = b2Vec2(-10.f, 275.f) - ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition();
			}
			else
			{
				newPos = b2Vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x, 275.f) - ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition();
				ECS::GetComponent<Transform>(bossHpBar).SetPosition(vec3(ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition().x *1.1 , 800.f, 0.03f));
				ECS::GetComponent<Transform>(bossInBar).SetPosition(vec3(ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition().x * 1.1, 552.f, 0.03f));

			}
			


			screenShakeTimer = 0;
		}
		else
		{
			newPos = b2Vec2(-22.f + ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetLinearVelocity().x, 280.f + ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetLinearVelocity().y) - ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition();
		}

		ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());


		float length = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);
		newPos = b2Vec2(newPos.x, newPos.y);

		b2Vec2 speed = b2Vec2(1000, 1000);

		ECS::GetComponent<PhysicsBody>(playerFollow).GetBody()->SetLinearVelocity(b2Vec2(newPos.x * speed.x * Timer::deltaTime, newPos.y * speed.y * Timer::deltaTime));
		ECS::GetComponent<PhysicsBody>(playerFollow).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 45.f * Timer::deltaTime), true);




		if (Input::GetKeyDown(Key::Shift) && Input::GetKey(Key::D) && airDashCounter >= 0.f)
		{
			screenShakeTimer = screenShakeTimerDefault;
			
		}
		else if (Input::GetKeyDown(Key::Shift) && Input::GetKey(Key::A) && airDashCounter >= 0.f)
		{
			screenShakeTimer = screenShakeTimerDefault;

		}
		else
		{
			screenShakeTimer -= 1 * Timer::deltaTime;
		}

	}
	else
	{
		if (fov > 71)
		{
			fov -= 2.5f * Timer::deltaTime;
			ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);

		}
		//std::cout << "\nNORMAL";
		b2Vec2 newPos = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition() + b2Vec2(mousePosX / 30, (mousePosY / 15) - 10) - ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition();
		float length = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);
		newPos = b2Vec2(newPos.x, newPos.y);

		b2Vec2 speed = b2Vec2(10, 10);
		ECS::GetComponent<PhysicsBody>(playerFollow).GetBody()->SetLinearVelocity(b2Vec2(newPos.x * speed.x, newPos.y * speed.y));
	}
}



void BossPhase1::updateUI()
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
		//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetCollisionBit(TRIGGER);
		invincibilityTime -= 1.f * Timer::deltaTime;
	}
	else
	{
		//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetCollisionBit(PLAYER);

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

void BossPhase1::SavePlayerLoc() {
	std::ofstream fstre;
	std::string fileLoc = "assets/PlayerSaves/File2.txt";

	int tempx = -624.f;
	int tempy = 1268.f;

	fstre.open(fileLoc);
	if (!fstre) {
		std::cout << "Failed to save player location!" << std::endl;
	}
	else {
		fstre << tempx << " " << tempy;
	}
}

b2Vec2 BossPhase1::LoadPlayerLoc() {
	std::fstream fstre;
	std::string fileLoc = "assets/PlayerSaves/File2.txt";
	int tempx = 0;
	int tempy = 0;

	fstre.open(fileLoc);
	if (!fstre) {
		std::cout << "Failed to load player location!" << std::endl;
	}
	else {
		fstre >> tempx;
		fstre >> tempy;
	}

	return b2Vec2(tempx,tempy);
}

void BossPhase1::Update()
{

	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto pl = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	player.Update();
	player.canYouFuckingJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).m_canJump;
	player.haveYouPressedSpace = spacePressed;
	if (player.m_attacking && player.haveYouPressedSpace == false) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(256);
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetHeight(128);
	}
	else if (player.m_attacking && player.haveYouPressedSpace == true) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(64);
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetHeight(64);
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

	diffTime2 = (clock() - startTime2) / (double)(CLOCKS_PER_SEC);

	bossDiffTime = (clock() - bossStartTime) / (double)(CLOCKS_PER_SEC);

	punDiffTime = (clock() - punStartTime) / (double)(CLOCKS_PER_SEC);

	//std::cout << "Vel: " << ECS::GetComponent<PhysicsBody>(boss).GetVelocity().x << " Frame: " << bossFrameNum << " Dif Time: " << bossDiffTime << std::endl;

	/*backgtimer += 1 * Timer::deltaTime;
	if (backgtimer > 1.25) {
		backgtimer = 0;
	}

	if (backgtimer > 1)
	{
		ECS::GetComponent<Sprite>(bg5).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bg4).SetTransparency(0.f);
	}
	else if (backgtimer > 0.75)
	{
		ECS::GetComponent<Sprite>(bg4).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bg3).SetTransparency(0.f);

	}
	else if (backgtimer > 0.5)
	{
		ECS::GetComponent<Sprite>(bg3).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bg2).SetTransparency(0.f);

	}
	else if (backgtimer > 0.25)
	{
		ECS::GetComponent<Sprite>(bg2).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bg1).SetTransparency(0.f);

	}
	else if (backgtimer < 0.25)
	{
		ECS::GetComponent<Sprite>(bg1).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bg5).SetTransparency(0.f);

	}*/

	if (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x > -600 && !activatePunisher) {
		activatePunisher = true;
		SavePlayerLoc();
		health = 6;
		punStartTime = clock();
		ECS::GetComponent<Sprite>(puni3Text).SetTransparency(1.f);
	}

	if (activatePunisher && punDiffTime > 0.12) {
		std::string fileName = "punisher/Punisher-Cooking1.png";
		punStartTime = clock();
		punCurFrame += 1;

		switch (punCurFrame) {
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

		ECS::GetComponent<Sprite>(punisher).LoadSprite(fileName, 128, 128);
	}

	
	if (startup == false)
	{

		startup = true;

	}

	if (health <= 0) {
		std::cout << "Resetting player location at: " << LoadPlayerLoc().x << " " << LoadPlayerLoc().y << std::endl;
		health = 6;
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetPosition(LoadPlayerLoc());
		bossStarted = false;
		fov = 70.f;
		ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
		if (doorDeleted == false)
		{
			if (phase2 == true)
			{

				ECS::GetComponent<PhysicsBody>(boss).SetHealth(20); //If player dies after boss is killed, game WILL crash. Honestly boss should never be deleted, just moved.

			}
			else
			{

				ECS::GetComponent<PhysicsBody>(boss).SetHealth(ECS::GetComponent<PhysicsBody>(boss).GetMaxHealth()); //If player dies after boss is killed, game WILL crash. Honestly boss should never be deleted, just moved.

			}
		}
	}



	cameraTrackPlayer();
	updateUI();
	CheckTransition();
	if (phase2 == false)
	{
		if (ECS::GetComponent<PhysicsBody>(boss).GetHealth() <= 30)
		{
			phase2 = true;
			//PhysicsBody::m_bodiesToDelete.push_back(phase1Wall);
			PhysicsBody::m_bodiesToDelete.push_back(phase1Wall2);
			makeBox(139.f, 421.f, 0.02f, 0.f, 92.f, 27.f);
			makeBox(-180.f, 421.f, 0.02f, 0.f, 92.f, 27.f);
			makeBox(170.f, 391.f, 0.02f, 0.f, 29.f, 16.f);
			ECS::GetComponent<Sprite>(ov1).SetTransparency(0.f);
			ECS::GetComponent<Sprite>(ov2).SetTransparency(1.f);



		}
	}
	//std::cout << "\n" << airDashCounter;

	if (phase2 == true)
	{
		if (ECS::GetComponent<PhysicsBody>(boss).spawnBatsNow == true)
		{
			makeBat(-23.f,641.f , 0.02f, 0, 10, 10);
			makeBat(-23.f, 651.f, 0.02f, 0, 10, 10);
			makeBat(-23.f, 631.f, 0.02f, 0, 10, 10);

			ECS::GetComponent<PhysicsBody>(boss).spawnBatsNow = false;
		}


	}

	//Door Deletion
	if (ECS::GetComponent<PhysicsBody>(boss).GetHealth() <= 0)
	{
		if (doorDeleted == false)
		{
			PhysicsBody::m_bodiesToDelete.push_back(exitDoor);
			doorDeleted = true;
			ECS::GetComponent<Sprite>(bossHpBar).SetTransparency(0.f);
			ECS::GetComponent<Sprite>(bossInBar).SetTransparency(0.f);


		}
	}

	//Lava
	if (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y <= 260.f && ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x <= 1000.f)
	{
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->SetLinearVelocity (b2Vec2(0.f, 50000.f));
		if (startAttackCooldown == false)
		{
			health--;
			startAttackCooldown = true;
		}

	}


}

void BossPhase1::GUI()
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

void BossPhase1::GUIWindowUI()
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

void BossPhase1::GUIWindowOne()
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

void BossPhase1::GUIWindowTwo()
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

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
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

void BossPhase1::RunLevelEditor()
{
	float scaleSpeed = 10;
	b2Vec2 wMousePos;
	wMousePos = (b2Vec2(mousePosX / 5, mousePosY / 5));
	wMousePos += b2Vec2(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition().x, ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition().y);
	ECS::GetComponent<Sprite>(rayMarker).SetTransparency(0.9f);
	ECS::GetComponent<Transform>(rayMarker).SetPosition(wMousePos.x, wMousePos.y, 2);

	ECS::GetComponent<Sprite>(editorEnabled).SetTransparency(0.9f);
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
		makeBox(wMousePos.x, wMousePos.y, 0.02f, 0, 50, 50);
	}
	else if (Input::GetKeyDown(Key::Two))
	{
		entitiesCreated = true;
		makeBox2(wMousePos.x, wMousePos.y, 0.02f, 0, 64, 64);

	}
	else if (Input::GetKeyDown(Key::Three))
	{
		entitiesCreated = true;
		makeBox2(wMousePos.x, wMousePos.y, 0.02f, 0, 128, 128);


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

	else if (Input::GetKeyDown(Key::Nine))
	{
		entitiesCreated = true;
		makeSmallMush(wMousePos.x, wMousePos.y, 0.05f, 0, 50, 48);

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


void BossPhase1::KeyboardHold()
{

	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());


	int maxVel = 120;

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
	if (Input::GetKeyDown(Key::B))
	{
		//bossStarted = true;


	}

}

void BossPhase1::KeyboardDown()
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

void BossPhase1::KeyboardUp()
{


}

void BossPhase1::MouseMotion(SDL_MouseMotionEvent evnt)
{

	mousePosX = evnt.x - (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().x / 2);
	mousePosY = (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y - evnt.y) - (ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetWindowSize().y / 2);

}

void BossPhase1::MouseClick(SDL_MouseButtonEvent evnt)
{
	mouseEvnt = evnt;
}
