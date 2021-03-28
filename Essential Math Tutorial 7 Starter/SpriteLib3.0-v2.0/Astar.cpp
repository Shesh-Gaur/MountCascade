#include "Utilities.h"
#include <iostream>
#include <vector>
#include "Astar.h";
#include "RayCastCallback.h"
using namespace std;
char junk;

//AI Grid

node allNodes[numOfNodes];

int openSet[numOfNodes];
int openCount = 0;

int closedSet[numOfNodes];
int closedCount = 0;

node pathSet[numOfNodes];
int pathCount;
int nodeCount = 0;

int startNode,endNode;

float getDeltaTime()
{
	return Timer::deltaTime;

}

int getCurrentClock()
{
	return Timer::currentClock;

}
int getNodeCount()
{
	return nodeCount;


}

node *getNodeList()
{
	return allNodes;

}

node* getPathSet()
{
	return pathSet;

}
void updateNbors(b2World* m_physicsWorld) //Finds the neighboring nodes to the nodes being checked. //Find the 8 closest nodes for every single node (all neighbors should be calculated at start of program)
{


	//IT WORKS! All nbor's nodes will always be accesible.
	for (int i = 0; i < nodeCount; i++)
	{
		for (int p = 0; p < nodeCount; p++)
		{
			if (p != i)
			{
				if (allNodes[p].position.x <= allNodes[i].position.x + gap && allNodes[p].position.x >= allNodes[i].position.x - gap)
				{
					if (allNodes[p].position.y <= allNodes[i].position.y + gap && allNodes[p].position.y >= allNodes[i].position.y - gap)
					{
						RayCastCallback obstacleRay;
						m_physicsWorld->RayCast(&obstacleRay, allNodes[i].position, allNodes[p].position);
						if (obstacleRay.m_fixture == nullptr)
						{
							allNodes[i].neighbors[allNodes[i].nborsCount] = p; //stores index of nbor in node array
							allNodes[i].nborsCount++;
						}
					}

				}
			}

		}
	}

}

void findStartAndEnd(b2Vec2 start, b2Vec2 end)
{
	//for (int p = 0; p < nodeCount; p++)
	//{
	//	//Can just check if a node is in 50 unit box around player to set it as start/end, then only calculate distance if ya cant find a node.(only do this is the current method is slow)
	//}

	//Find Start


	closedCount = 0;
	openCount = 0;
	pathCount = 0;
	float min = 1000;
	int minIndex = 0;

	for (int p2 = 0; p2 < nodeCount; p2++)
	{
		allNodes[p2].parent = 0;
		allNodes[p2].reset();
		b2Vec2 direction = allNodes[p2].position - start;
		float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (distance < min)
		{
			min = distance;
			minIndex = p2;
		}
	}

	startNode = minIndex;
	allNodes[minIndex].makeStart();

	//Find End
	min = 1000;
	minIndex = 0;

	for (int p3 = 0; p3 < nodeCount; p3++)
	{
		b2Vec2 direction = allNodes[p3].position - end;
		float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (distance < min)
		{
			min = distance;
			minIndex = p3;
		}
	}

	endNode = minIndex;
	//cout << minIndex;
	allNodes[minIndex].makeEnd();
}

//Calculates G cost 
int g(node p1) 
{
	b2Vec2 direction = p1.position - allNodes[startNode].position;
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	return distance;

}


//Calculates H cost
int h(node p1)
{
	b2Vec2 direction = p1.position - allNodes[endNode].position;
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	return distance;
}


//Calculates F cost
int f(node p1)
{
	int fCost = p1.gCost + p1.hCost;
	return fCost;
}

////Generates the AI grid
void makeGrid(b2Vec2 position)
{
	node Node(position);
	allNodes[nodeCount] = Node;
	nodeCount++;
}

void resetGrid()
{
	nodeCount = 0;
	
	//for (int i = 0; i < numOfNodes; i++)
	//{
	//	allNodes[i] = ;
	//	
	//}
}
int getPathCount()
{
	return pathCount;
}

void resetPathcount()
{
	pathCount = 0;
}

//Compares nodes in the open set, returns the one with the lowest f cost.
int compareOpen()
{
	
	node min;
	int minin = 0;
	min.fCost = 100000000000;
	for (int i = 0; i < openCount; i++)
	{
		allNodes[openSet[i]].hCost = h(allNodes[openSet[i]]);
		allNodes[openSet[i]].gCost = g(allNodes[openSet[i]]);
		allNodes[openSet[i]].fCost = f(allNodes[openSet[i]]);
		
		
	}

	for (int i = 0; i < openCount; i++)
	{
		if (allNodes[openSet[i]].fCost < min.fCost)
		{
			min = allNodes[openSet[i]];
			minin = i;
		}

	}
	int oh = minin;
	//int total = nodeCount - minin;
	return oh;


	
	



}

//Sequences the final path by tracing back the parents of the end node
void sequencePath(int selected)
{
	

	//I think this stops one node before end maybe?
	while (allNodes[selected].isStart() == false)
	{
		//std::cout << "\n" << selected;
		allNodes[selected].makePath();
		pathSet[pathCount] = allNodes[selected];
		pathCount++;
		
		//std::cout << "\n\n" << allNodes[selected].position.x << " " << allNodes[selected].position.y;
		
		selected = allNodes[selected].parent;

	}

}

void CalculatePath()
{	
	if (startNode != endNode)
	{
		closedSet[closedCount] = startNode;
		//cout << startNode.position.x;
		closedCount++;
		//Algorithm implimentation
		for (int p = 0; p < closedCount; p++)
		{
			//std::cout << "\n\n";
			//std::cout << "\n " << allNodes[closedSet[p]].position.x << " " << allNodes[closedSet[p]].position.y;

			for (int g = 0; g < allNodes[closedSet[p]].nborsCount; g++)
			{
				//Checks if the end node is a neighbor / has been found

				if (allNodes[allNodes[closedSet[p]].neighbors[g]].isEnd() == true)
				{
					allNodes[allNodes[closedSet[p]].neighbors[g]].parent = closedSet[p];
					//cout << "\n\nTARGET HAS BEEN FOUND";
					sequencePath(allNodes[closedSet[p]].neighbors[g]);
					closedCount = 0;
					break;

				}

				//Add to open set
				if (allNodes[allNodes[closedSet[p]].neighbors[g]].isBlank())
				{
					//allNodes[allNodes[closedSet[p]].neighbors[g]].parent = closedSet[p];
					//Ahh prob must be here. Parents keep getting set to 0?

					allNodes[allNodes[closedSet[p]].neighbors[g]].makeOpen();
					openSet[openCount] = allNodes[closedSet[p]].neighbors[g];
					allNodes[allNodes[closedSet[p]].neighbors[g]].parent = closedSet[p];
					//std::cout << "\n" << allNodes[allNodes[closedSet[p]].neighbors[g]].parent;
					openCount++;
				}
			}
			//Add to closed set
			//cout << "\nCC:" << compareOpen().position.x << " "<< compareOpen().position.y;
			int ind = compareOpen();
			closedSet[closedCount] = openSet[ind];
			allNodes[closedSet[closedCount]].makeClosed();
			closedCount++;
			//cout << "\n";
			//Removes node from open set so it has to shift over everything?
			for (ind; ind < openCount; ind++)
			{
				openSet[ind] = openSet[ind + 1];

			}
			openCount--;
			//HOLY FUCK IT WORKS!!
		}
		//drawGrid();


		//fill_n(closedSet, 576, 0);
		//fill_n(openSet, 576, 0);
	}
}



