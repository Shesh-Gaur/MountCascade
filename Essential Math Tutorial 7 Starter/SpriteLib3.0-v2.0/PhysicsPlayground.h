#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>
#include <fstream>
#include <iostream>
class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	bool spacePressed;
	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

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
	void makeNode(float xPos, float yPos,float transparency);
	void makeBat(float xPos, float yPos, float zPos, float rotation, float width, float height);	
	void RunLevelEditor();
	void cameraTrackPlayer();
	void ZoomCamera();
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

	int healthBar;
	int dashBar;
	int dummy;
	
};
