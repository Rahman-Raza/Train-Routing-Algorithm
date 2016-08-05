// Author: Sean Davis

#include "TrainRunner.h"
#include "train.h"
#include "StackLi.h"
#include "BinaryHeap.h"
#include "LinkedList.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

Train::Train(Station *stations, int numStations, int numCars) 
{
	ourStations = new Station[numStations];
	memcpy(ourStations, stations, numStations*sizeof(Station));
	stationCount = numStations;
	carCount = numCars;
	ourCars = new Car[numCars];
	operationNum = 0;
	simulatedTime = 0;
	carryingTracker = 0;
	dropOffCount = 0;
	rootNode = 0;
} // Train()


Train::DIJ::DIJ(int station)
{
	currentStation = station;
} //DIJ()


void Train::run(Car *cars, Operation *operations, int *numOperations)
{
	memcpy(ourCars, cars, carCount*sizeof(Car));

	for (int i = 0; i < carCount; i++)  //putting all the cars at their destinations
	{
		ListItr <int> listItr = stationCars[ourCars[i].origin].zeroth(); 
		//get the iterator for the stationCars array element which is a linked list
		stationCars[ourCars[i].origin].insert(i, listItr); 
		 //inserting cars into the linked list of the station they belong to.  
       
        // cout << "Car " << stationCars[ourCars[i].origin].find(i).retrieve() << " goes to " << ourCars[i].origin <<endl;  
	}

	*numOperations = 0;

	StackLi <int> dijStack;
	dijStack = runDij(ourStations,4,3);
	return;
} // run()



StackLi <int> Train::runDij (Station *ourStation, int rootNode, int destination)
{
	rootNode = 0;
	int pv[20000];
	int known[20000];
	unsigned int dv[20000];
	BinaryHeap <stationheap> minHeap (20000);
	StackLi <int> stationStack;
	
	pv[rootNode] = -1;
	dv[rootNode] = 0;
	known[rootNode] = 1;
	
	//This loop creates new Heap Objects for all nodes adjacent to the root node, and
	//initializes their values in the respective arrays
	for (int i =0; i < ourStation[rootNode].adjCount; i++)
	{
			//Create new node
		stationheap newHeapNode(ourStation[rootNode].adjacent[i], ourStation[rootNode].distances[i]);
			//insert new node to heap
		minHeap.insert(newHeapNode);
			//update new node's pv to root node
		pv[ourStation[rootNode].adjacent[i]] = rootNode;
			//update new node's dv to rootNode->new node distance
		dv[ourStation[rootNode].adjacent[i]] = ourStation[rootNode].distances[i];
			//update new node to false (isntead of "nothing")
		known[ourStation[rootNode].adjacent[i]] = 0;
	}

	//Now that we have first rootNode node and the set of adjacent nodes, we can
	//find the shortest path of those nodes and build Dijkstra's from there

	//This loop runs until destination is found
	while (known[destination] != 1)
	{
	//Find shortest path, set that as current node, set its known to true
		stationheap currNode = minHeap.findMin();
		known[currNode.location] = 1;
		
		
	//This loop adds new nodes from currNode adjacents to the heap, and sets the known, pv and dv arrays of those adjacents
		for (int i = 0; i < ourStation[currNode.location].adjCount; i++)
		{
		//Create new heap objects from currNode adjacents and insert them to heap
			stationheap currAdj(ourStation[currNode.location].adjacent[i], 
				ourStation[currNode.location].distances[i] + dv[currNode.location]);
			minHeap.insert(currAdj);
			
		//Check if this node is the destination, if it is we will end Dijksta's Algo (runDij) here
			if(currAdj.location == destination)
			{
				int pv2;
				known[destination] = 1; //to meet while loop termination condition
				//Push the destination station's pv ID to stack
				stationStack.push(currAdj.location);

				pv2 = currNode.location;
				do //push all pvs to stack
				{
					//Push current pv ID to stack
					stationStack.push(pv2);
					pv2 = pv[pv2];
					 //go down to this pv's pv

				} while(pv2 != -1);
			
				return stationStack; //now stack contains all IDs, including dest ID, except rootNode ID
			}
		
		//this node was not the destination so:
			else
			{
			
			// RUN SPECIAL OPS*********************************************
				//If the node has been visited, see if you are coming from a shorter path than it currently has
				if(known[ourStation[currNode.location].adjacent[i]] == 0) //a value of 0 indicates prev. visitation
				{
					//then it HAS to have a dv, so check that against the addition we would add (current node's dv + distance to new node)
					if(dv[ourStation[currNode.location].adjacent[i]] > (unsigned int)(ourStation[currNode.location].distances[i] + currNode.distance))
					{
						//dv + distance is smaller so set that as the new dv and set new pv
						dv[ourStation[currNode.location].adjacent[i]] = ourStation[currNode.location].distances[i] + currNode.distance;
						pv[ourStation[currNode.location].adjacent[i]] = currNode.location;
					}
				}
				
			//RUN NO OPS***************************************************
				//it's marked known, don't do anything to it
				else if (known[ourStation[currNode.location].adjacent[i]] == 1)
				{
					break; //go to next while loop
				}
				
			//RUN DEFAULT OPS**********************************************
				//else, the node wasn't visited
				else
				{
					known[ourStation[currNode.location].adjacent[i]] = 0;
					dv[ourStation[currNode.location].adjacent[i]] = ourStation[currNode.location].distances[i] + currNode.distance;
					pv[ourStation[currNode.location].adjacent[i]] = currNode.location;
				}
			}
		}//for (iterator < adjCount)
	}//while (destination ! found)
	return stationStack;  //incomplete return
}//runDij()

void Train::move(Operation *operations,  int simTime, int ID, int operationNum)
{
	operations[operationNum].operation = 'M';
	
	operations[operationNum].time = simTime;
	operations[operationNum].ID = ID;
}

void Train::pickUp (Operation *operations,  int simTime, int ID, int operationNum)
{

	operations[operationNum].operation = 'P';
	operations[operationNum].time = simTime;
	operations[operationNum].ID = ID;
}


void Train::dropOff (Operation *operations,  int simTime, int ID, int operationNum)
{
operations[operationNum].operation = 'D';
	operations[operationNum].time = simTime;
	operations[operationNum].ID = ID;
}
