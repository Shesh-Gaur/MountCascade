#pragma once

const int gridWidth = 10;
const int numOfNodes = gridWidth * gridWidth;
const int gap = 50;
class node
{
public:
	b2Vec2 position = b2Vec2(0, 0);
	int neighbors[8];
	int parent;
	float hCost = 0, gCost = 0, fCost = 0;
	char type = '.';
	int nborsCount = 0;


	//Getters
	bool isClosed()
	{
		if (type == 'X')
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool isBlank()
	{
		if (type == '.')
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool isOpen()
	{
		if (type == 'O')
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool isStart()
	{
		if (type == 'A')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool isEnd()
	{
		if (type == 'B')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool isPath()
	{
		if (type == '*')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void reset()
	{
		type = '.';
	}

	//Setters
	void makeClosed()
	{
		type = 'X';
	}

	void makeOpen()
	{
		type = 'O';
	}



	void makeStart()
	{
		type = 'A';
	}

	void makeEnd()
	{
		type = 'B';
	}

	void makePath()
	{
		type = '*';
	}

	void draw(node allNodes[numOfNodes])
	{
		//grid[row][col].type = type; //IDK why this exists
	}

	node(b2Vec2 pos = b2Vec2(0, 0))
	{

		position = pos;

	}



};
void CalculatePath();
void makeGrid(b2Vec2 position);
void updateGrid(int playerX, int playerY);

int getPathRow(int count);
int getPathCol(int count);
int getPathCount();
void resetPathcount();
void updateNbors(b2World* m_physicsWorld);
//node allNodes[numOfNodes];
void findStartAndEnd(b2Vec2 start, b2Vec2 end);
int getNodeCount();
node *getNodeList();
node* getPathSet();
float getDeltaTime();
int getCurrentClock();
