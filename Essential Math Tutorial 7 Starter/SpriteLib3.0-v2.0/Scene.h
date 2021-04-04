#ifndef __SCENE_H__
#define __SCENE_H__

#include "JSON.h"
#include "ECS.h"
#include "Timer.h"
#include "Xinput.h"
#include "MainEntities.h"
#include <SDL2/SDL.h>


class Scene
{
public:
	Scene() { m_sceneReg = new entt::registry(); m_physicsWorld = new b2World(m_gravity); }
	Scene(std::string name);
	~Scene() { }

	void Unload();

	//Each scene will need to have a different
	//init, as each scene's contents will be different
	virtual void InitScene(float windowWidth, float windowHeight);

	virtual void Update();

	virtual void GUI();

	void AdjustScrollOffset();
	void CreateCameraEntity(bool mainCamera, float windowWidth, float windowHeight, float left, float right, float bottom, float top, 
								float zNear, float zFar, float aspectRatio, bool vertScroll=false, bool horizScroll=false);

	//Gamepad Input
	//Because these are virtual you can override them in your inherited classes.
	//The same way you do for Update().
	virtual void GamepadStroke(XInputController* con) { };
	virtual void GamepadUp(XInputController* con) { };
	virtual void GamepadDown(XInputController* con) { };
	virtual void GamepadStick(XInputController* con) { };
	virtual void GamepadTrigger(XInputController* con) { };
	virtual void KeyboardHold() { };
	virtual void KeyboardDown() { };
	virtual void KeyboardUp() { };
	
	//Mouse Input
	//Because these are virtual you can override them in your inherited classes.
	//The same way you do for Update().
	virtual void MouseMotion(SDL_MouseMotionEvent evnt) { };
	virtual void MouseClick(SDL_MouseButtonEvent evnt) { };
	virtual void MouseWheel(SDL_MouseWheelEvent evnt) { };

	//Get the scene registry
	entt::registry* GetScene() const;
	//Set the scene registry
	void SetScene(entt::registry& scene);

	//Gets the background color of the scene
	vec4 GetClearColor() const;
	//Sets the background color of the scene
	void SetClearColor(vec4 clearColor);

	//Gets the name of the scene
	std::string GetName() const;
	//Sets the name of the scene
	void SetName(std::string name);
	
	//Gravity
	b2Vec2 GetGravity() const;
	void SetGravity(b2Vec2 grav);
	//Physics world
	b2World& GetPhysicsWorld();
	void CheckTransition();
	void makeLoadingScreen();
	void PlayerMovement();
	//Set window size (makes sure the camera aspect is proper)
	void SetWindowSize(float windowWidth, float windowHeight);
protected:
	b2World* m_physicsWorld = nullptr;
	b2Vec2 m_gravity = b2Vec2(float32(0.f), float32(0.f));

	vec4 m_clearColor = vec4(0.15f, 0.33f, 0.58f, 1.f);
	b2Vec2 playerLoadingPos;
	bool transitionStarted = false;
	entt::registry* m_sceneReg = nullptr;	
	std::string m_name = "Default Name";
	float defaultForce = 160000.f;
	float curVel = 0.f;
	bool spacePressed;
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

	int playerHpDefault = 6;
	int playerHp = playerHpDefault;

	int health = 6;
	float invincibilityTimeDefault = 3.0f;
	float invincibilityTime = 0;
	bool isTouchingIceWall = false;
	bool hasChargeJump = false;
private:
	//Hello world entity number
	int m_helloWorldSign = 0;
};

#endif // !__SCENE_H__
