#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>
#include <fstream>
#include <iostream>

class CascadeVillage : public Scene
{
public:
	CascadeVillage(std::string name);

	
	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;
	void SavePlayerLoc();
	b2Vec2 LoadPlayerLoc();

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();


	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;
	void MouseMotion(SDL_MouseMotionEvent evnt) override;
	void MouseClick(SDL_MouseButtonEvent evnt) override;
	void writeAutoSaveFile(int file);
	void writeSaveFile();
	void readSaveFile();
	void makeBox(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeBox2(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeDummy(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeMushroom(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeStalagmite1(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeStalagmite2(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeNode(float xPos, float yPos, float transparency);
	void makeBat(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeIceWall(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeTree(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeRock(float xPos, float yPos, float zPos, float rotation, float width, float height);
	void makeRock2(float xPos, float yPos, float zPos, float rotation, float width, float height);


	void RunLevelEditor();
	void cameraTrackPlayer();
	void ZoomCamera();
	void updateUI();
	void loadNear();

	float cameraZoomDefault = 75.f;
	float cameraZoom = 75.f;
	float aRatio;
	float nPlane = 3.0f;
	float fov = 70.f;

	//Important Variables
	float mousePosX, mousePosY;
	bool levelEditor, levelEditorStartup = false, startup = false;

	float saveUITimer;
	bool objectAlreadySelected;
	bool entitiesCreated = false;
	SDL_MouseButtonEvent mouseEvnt;
	float autosaveTimer = 0.f;
	int autosaveInterval = 60;

	int gLength = 10, gWidth = 10;
	b2Vec2 pastPlayerPosition = b2Vec2(999, 999);
	int nextNode = 1;
	float pathFindTimerDefault = 2.f;
	float pathFindTimer = pathFindTimerDefault;
	float switchNodeTimer = 0.f;
	float attackCooldownTimerDefault = 1.f;
	float attackCooldownTimer = attackCooldownTimerDefault;
	bool startAttackCooldown = false;
	int lastHealth;
	int lastDash;

	int houseEntity;
	float hpTrans = 1.f;
	float dashTrans = 1.f;

protected:
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;
	//Editor UI
	int rayMarker;
	int editorEnabled;
	int editZone;
	int changesSaved;
	int test;
	int playerFollow;
	int puzzleWall1;
	int puzzleWall2;
	int selectedEntity;

	int mushroomBoss;
	int mushroomBoss2;
	int zoomTrigger1;
	int attackTrigger1;
	int translateTrigger1;


	int healthBar;
	int dashBar;
	int dummy;

	int puni;
	int puniText;

};
