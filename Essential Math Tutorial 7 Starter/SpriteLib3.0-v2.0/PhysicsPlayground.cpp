#include "PhysicsPlayground.h"
#include "Utilities.h"
#include "RayCastCallback.h"
#include <random>
#include <fstream>
#include <iostream>
#include "Astar.h"
#include "ZoomTrigger.h"
#include "ZoomConfirm.h"
PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	
	m_gravity = b2Vec2(0.f, -45.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
	


}
float cameraZoomDefault = 75.f;
float cameraZoom = 75.f;
float aRatio;
float nPlane = 3.0f;
float fov = 70.f;
void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	aRatio = aspectRatio;
	EffectManager::CreateEffect(EffectType::Vignette, windowWidth, windowHeight);
	EffectManager::CreateEffect(EffectType::Sepia, windowWidth, windowHeight);
	EffectManager::RemoveEffect(EffectManager::GetSepiaHandle()); //added this to get rid of sephia


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
		std::string fileName = "ui/Health3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 240, 125);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.8f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));
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
		tempDef.position.Set(float32(0), float32(0));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, OBJECTS,PICKUP,0.f,0.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);

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
		tempDef.position.Set(float32(0.f), float32(100.f));

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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3200, 1800);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(300.f, 150.f,-55.f));
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
		std::string fileName = "Cave_background1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1920, 1080);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(1100.f, 300.f, -50.f));
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(600.f, 300.f, -49.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		mushroomBoss = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Set up the components
		std::string fileName = "BeachBall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.02f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(400), float32(500));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, ENEMY, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.f, 0.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);
		tempPhsBody.SetName("Bat");

	}
	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		mushroomBoss2 = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Set up the components
		std::string fileName = "BeachBall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 10);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.02f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(500), float32(500));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), vec2(0.f, 0.f), false, ENEMY, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.f, 0.f);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(0);
		tempPhsBody.SetName("Bat");
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 56, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
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

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(40.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, ENEMY);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

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

//Important Variables
float mousePosX, mousePosY;
bool levelEditor,startup = false;

float saveUITimer;
bool objectAlreadySelected;
bool entitiesCreated = false;
SDL_MouseButtonEvent mouseEvnt;
float autosaveTimer = 0.f;
int autosaveInterval = 60;
float defaultForce = 160000.f;
float jumpForce = defaultForce;
int airJumpDefault = 1;
int airJumpCounter = airJumpDefault;
bool spaceReleased = false;
//bool spacePressed = false;
bool canDash = false;
float dashAmount = 60.f;
float airDashDefault = 3.f;
float airDashCounter = airDashDefault;
bool loadStarted = false;
int playerHpDefault = 3;
int playerHp = playerHpDefault;
int dummyHpDefault = 10;
int dummyHp = playerHpDefault;
int health = 3;

void PhysicsPlayground::writeAutoSaveFile(int file)
{
	char x;
	unsigned int entityNum = 0;
	std::ofstream editorSaveFile;
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
		if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetBody()->GetType() == b2_staticBody)
		{
			editorSaveFile << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x;
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().y;
			editorSaveFile << "\t" << ECS::GetComponent<Transform>((int)body->GetUserData()).GetPosition().z;
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetRotationAngleDeg();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetWidth();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetHeight();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetFileName() << "\n";


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


	for (int f = 0; f < m_physicsWorld->GetBodyCount(); f++)
	{
		if (ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetBody()->GetType() == b2_staticBody)
		{
			editorSaveFile << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().x;
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetPosition().y;
			editorSaveFile << "\t" << ECS::GetComponent<Transform>((int)body->GetUserData()).GetPosition().z;
			editorSaveFile << "\t" << ECS::GetComponent<PhysicsBody>((int)body->GetUserData()).GetRotationAngleDeg();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetWidth();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetHeight();
			editorSaveFile << "\t" << ECS::GetComponent<Sprite>((int)body->GetUserData()).GetFileName() << "\n";
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

	}
	editorSaveFile.close();
}


void PhysicsPlayground::cameraTrackPlayer()
{

	b2Vec2 newPos = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition() - ECS::GetComponent<PhysicsBody>(playerFollow).GetPosition();
	float length = sqrt(newPos.x * newPos.x + newPos.y * newPos.y);
	newPos = b2Vec2(newPos.x, newPos.y);

	b2Vec2 speed = b2Vec2(1000, 1000);

	ECS::GetComponent<PhysicsBody>(playerFollow).GetBody()->SetLinearVelocity(b2Vec2(newPos.x * speed.x * Timer::deltaTime, newPos.y * speed.y * Timer::deltaTime));
	ECS::GetComponent<PhysicsBody>(playerFollow).GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 45.f * Timer::deltaTime), true);

}


void PhysicsPlayground::ZoomCamera()
{

		//if (fov > 60)
		//{
		//	fov -= 0.5f * Timer::deltaTime;

		//	ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
		//	ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());


		//}
	
	//else
	//{
	//	if (fov < 70)
	//	{
	//		fov += 0.5f * Timer::deltaTime;

	//		ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);
	//		ECS::GetComponent<Camera>(MainEntities::MainCamera()).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition());


	//	}


	//}

		//if (fov < 70)
		//{
		//	fov += 100.f * Timer::deltaTime;
		//	ECS::GetComponent<Camera>(MainEntities::MainCamera()).Perspective(fov, aRatio, nPlane, 1000.f);


		//}

}



int gLength = 10, gWidth = 5;
b2Vec2 pastPlayerPosition = b2Vec2(999,999);
int nextNode = 1;
float pathFindTimerDefault = 2.f;
float pathFindTimer = pathFindTimerDefault;
float switchNodeTimer = 0.f;
float attackCooldownTimerDefault = 1.f;
float attackCooldownTimer = attackCooldownTimerDefault;
bool startAttackCooldown = false;
int lastHealth;
int lastDash;

void PhysicsPlayground::Update()
{
	
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	player.Update();
	player.canYouFuckingJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer()).m_canJump;
	player.haveYouPressedSpace = spacePressed;
	if (player.m_attacking) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(128);
	}
	else if (player.m_dashing) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(448);
	}
	else if (player.m_dead) {
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(96);
	}
	else 
		ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).SetWidth(64);

	

	if (lastDash != airDashCounter) {
		if (airDashCounter >= 3) { //full dash bar
			std::string fileName = "ui/Dash3.png";
			player.canDash = true;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 65, 15);
		}
		else if (airDashCounter >= 2) {
			std::string fileName = "ui/Dash2.png";
			player.canDash = true;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 65, 15);
		}
		else if (airDashCounter >= 1) {
			std::string fileName = "ui/Dash1.png";
			player.canDash = true;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 65, 15);
		}
		else { //empty dash bar
			std::string fileName = "ui/Dash0.png";
			player.canDash = false;
			ECS::GetComponent<Sprite>(dashBar).LoadSprite(fileName, 65, 15);
		}
		lastDash = airDashCounter;
		//std::cout << std::endl << airDashCounter << std::endl;
	}

	if (lastHealth != health) {
		if (health >= 3) { //full health bar
			std::string fileName = "ui/Health3.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 65, 15);
		}
		else if (health >= 2) {
			std::string fileName = "ui/Health2.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 65, 15);
		}
		else if (health >= 1) {
			std::string fileName = "ui/Health1.png";
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 65, 15);
		}
		else { //empty health bar
			std::string fileName = "ui/Health0.png";
			player.m_dead = true;
			ECS::GetComponent<Sprite>(healthBar).LoadSprite(fileName, 65, 15);
		}
		lastHealth = health;
	}

	if (startup == false)
	{
		readSaveFile();
		
		for (int y = 0; y < (gWidth * 50); y+=50)
		{
			for (int x = 0 + 675; x < (gLength * 50)+ 675; x+=50)
			{
				RayCastCallback nodeRay;
				m_physicsWorld->RayCast(&nodeRay, b2Vec2(x,y) + b2Vec2(0, 50), b2Vec2(x, y));
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

				if (nodeRay.m_fixture == nullptr)
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
		player.theAttackTrigger = attackTrigger1;
		startup = true;
	}
	
	if (ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetContactList() != NULL)
	{
		if (ECS::GetComponent<PhysicsBody>((int)ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetBody()->GetContactList()->contact->GetFixtureB()->GetBody()->GetUserData()).GetName() == "Bat" && startAttackCooldown == false)
		{
			health--;
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
	
	cameraTrackPlayer();
	ZoomCamera();
	//std::cout << "\n" << airDashCounter;
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
					std::cout << "\n" << selectRay.m_fixture->GetBody()->GetPosition().x;
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
		makeBox2(wMousePos.x, wMousePos.y, 0.03f, 0, 50, 50);

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

	ECS::GetComponent<Transform>(dashBar).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition() + vec3(180, -100, 2));

	ECS::GetComponent<Transform>(healthBar).SetPosition(ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetPosition() + vec3(180, 130, 2));


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

		ECS::GetComponent<Transform>(editZone).SetPosition(player.GetPosition().x, player.GetPosition().y,2);
		
		auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
		float speed = 100.f;
		b2Vec2 vel = b2Vec2(0.f, 0.f);

		if (Input::GetKey(Key::A) && health > 0)
		{
			//player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * Timer::deltaTime * speed, 0.f), true);
			player.GetBody()->SetLinearVelocity(b2Vec2(-120.f * Timer::deltaTime * speed, player.GetBody()->GetLinearVelocity().y));
			
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
				
				player.GetBody()->GetFixtureList()->SetFriction(0);
			}
			else
			{
				player.GetBody()->GetFixtureList()->SetFriction(1.1f);
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
					airDashCounter-= 1;
				}

			}
		}
		if (Input::GetKey(Key::D) && health > 0)
		{
			//player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * Timer::deltaTime * speed, 0.f), true);
			player.GetBody()->SetLinearVelocity(b2Vec2(120.f * Timer::deltaTime * speed, player.GetBody()->GetLinearVelocity().y));

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
				
				player.GetBody()->GetFixtureList()->SetFriction(0);
			}
			else
			{
				player.GetBody()->GetFixtureList()->SetFriction(1.1f);
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
					airDashCounter-= 1;
				}
			}

		}

		if (canJump.m_canJump && !Input::GetKey(Key::D) && !Input::GetKey(Key::A)) {
			player.GetBody()->SetLinearVelocity(speed * vel + b2Vec2(player.GetBody()->GetLinearVelocity().x * 0.9f,
				player.GetBody()->GetLinearVelocity().y));
		}
		if (canJump.m_canJump)
		{
			spacePressed = false;

		}
		//std::cout << "\n" << airDashCounter;
		if (canJump.m_canJump && health > 0)
		{
			if (airDashCounter <= airDashDefault && loadStarted == true)
			{
				airDashCounter += 2.f * Timer::deltaTime;
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
						player.GetBody()->SetLinearVelocity(b2Vec2(player.GetBody()->GetLinearVelocity().x/2 , player.GetBody()->GetLinearVelocity().y/2 ));
						jumpForce += 60000 * Timer::deltaTime;
						spacePressed = true;
						//std::cout << "\n" << jumpForce;
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


}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::OEMMinus) && health > 0) health--;
	if (Input::GetKeyDown(Key::OEMPlus) && health < 3) health++;


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
