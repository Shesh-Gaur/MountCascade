#ifndef __BAT_H__
#define __BAT_H__

#include "BackEnd.h"

enum BatAnimEnums
{
	FLY_RIGHT,
	FLY_LEFT,

	DAMAGE_RIGHT,
	DAMAGE_LEFT
};

enum BatAnimTypes
{
	FLY = 0,
	DAMAGE = 2
};

enum BatAnimDir
{
	_RIGHT,
	_LEFT
};

class Bat
{
public:
	Bat();
	Bat(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr);

	void InitBat(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr);

	void Update();
	void MovementUpdate();
	void AnimationUpdate();

	//Basically, any animation OTHER than moving will not have a cancel, and we'll be checking whether or not that animation is done
	bool m_moving = false;
	//Have we locked the player from moving during this animation?
	bool m_locked = false;
	bool m_damaged = false;


private:
	void SetActiveAnimation(int anim);


	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our animation controller
	AnimationController* m_animController = nullptr;
	//A reference to our bat transform
	Transform* m_transform = nullptr;

	//Physics importance
	//A reference to our physics body
	PhysicsBody* m_physBody = nullptr;
	//Does this bat have physics?
	bool m_hasPhysics = false;

	//Default animation direction (feel free to change this to suit your game. If you're making a side-scroller, left or right would be better
	BatAnimDir m_facing = _RIGHT;
};

#endif // !__BAT_H__