#include "TransitionTrigger.h"
#include "ECS.h"
#include "Utilities.h"
#include "Game.h"
int previousEntity2 = 6969;
bool ran = false;
#undef main
Game theGame;
int main()
{
	//Create the Game
	

	//Initialize the game
	theGame.InitGame();

	//Runs the game until it returns :)
	return theGame.Run();
}

void TransitionTrigger::OnTrigger()
{
	Trigger::OnTrigger();
	
}


void TransitionTrigger::OnEnter()
{

	Trigger::OnEnter();
	ECS::GetComponent<Transform>(MainEntities::MainLoading()).SetPosition(vec3(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y,4));
	if (ECS::GetComponent<Sprite>(MainEntities::MainLoading()).GetTransparency() == 0.f)
	{
		ECS::GetComponent<Sprite>(MainEntities::MainLoading()).SetTransparency(0.1f);
	}
	if (ECS::GetComponent<Sprite>(MainEntities::MainLoading()).GetTransparency() >= 1.f)
	{
		std::cout << "\nHEY!";
		std::ofstream playerSaveFile;
		playerSaveFile.open("assets/PlayerSaves/File1.txt");
		float x, y;
		if (nextScene == 0)
		{
			theGame.switchSceneCascadeVillage = true;
			x = 1885;
			y = 283;


		}
		else if (nextScene == 1)
		{
			theGame.switchSceneMushroomCave = true;

			x = -850;
			y = 10;

		}

		playerSaveFile << x;
		playerSaveFile << "\t" << y;

		playerSaveFile.close();
	}
}


void TransitionTrigger::OnExit()
{

	Trigger::OnExit();
	theGame.switchSceneCascadeVillage = false;
	ran = false;
	
	

}

