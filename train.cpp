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
	currentAddress = 0;
} // Train()


Train::DIJ::DIJ(int station)
{
	currentStation = station;
} //DIJ()

int Train::findIndexOfMove(Station *ourStations, int station, int adjacent)
{
 for (int i = 0; i < ourStations[station].adjCount; i++)
 {
    if (ourStations[station].adjacent[i] == adjacent)
    	return i;
 }


}


void Train::run(Car *cars, Operation *operations, int *numOperations)
{
	memcpy(ourCars, cars, carCount*sizeof(Car));
	ListItr <int> listItr5 = allCars.zeroth();

	for (int i = 0; i < carCount; i++)  //putting all the cars at their destinations
	{
		ListItr <int> listItr = stationCars[ourCars[i].origin].zeroth(); 
		//get the iterator for the stationCars array element which is a linked list
		stationCars[ourCars[i].origin].insert(i, listItr); 

		allCars.insert(i,listItr5);
		

		 //inserting cars into the linked list of the station they belong to.  
       
        // cout << "Car " << stationCars[ourCars[i].origin].find(i).retrieve() << " goes to " << ourCars[i].origin <<endl;  
	}



	*numOperations = 0;

	



// if (!stationCars[currentAddress].isEmpty())  //check root for pickups
// {

// 	ListItr <int> listItr = stationCars[currentAddress].zeroth();
	

// 				while (!stationCars[currentAddress].isEmpty())  //pickup all the cars we can from this station
// 				{

					

// 					if(carryingTracker != 50)
// 					{
// 						int carToRemove = listItr.retrieve();
// 						carryingCars[carryingTracker] = carToRemove;
// 						stationCars[currentAddress].remove(carToRemove);
// 						operations[operationNum].time = 0;
// 						pickUp(operations,operations[operationNum].time, carryingCars[carryingTracker], operationNum);
// 						operationNum++;
// 						*numOperations+=1;
// 						carryingTracker++;
// 						listItr.advance();
// 					}
// 				}

// }


	for (int i = 0; i < ourStations[rootNode].adjCount; i++)  //check the rootnode (0)s adjacents for pickups
	{
			 currentAddress = ourStations[rootNode].adjacent[i];  
			if (!stationCars[currentAddress].isEmpty())    //check for first pickup
			{
				//cout<<"station: "<<ourStations[rootNode].adjacent[i]<<"has "<<endl;
				simulatedTime += ourStations[rootNode].distances[i];
				int ID = ourStations[rootNode].adjacent[i];
				move (operations, simulatedTime, ID,operationNum++);
				cout << "moved to: "<<ID << endl;
				*numOperations += 1;
				

				ListItr <int> listItr = stationCars[currentAddress].zeroth();
				listItr.advance();
				

				while (!stationCars[currentAddress].isEmpty())  //pickup all the cars we can from this station
				{
					

					if(carryingTracker != 50)
					{
						int carToRemove = listItr.retrieve();
						carryingCars[carryingTracker] = carToRemove;
						stationCars[currentAddress].remove(carToRemove);
						pickUp(operations,operations[operationNum - 1].time, carryingCars[carryingTracker], operationNum);
						cout << "pickedup:  "<<carryingCars[carryingTracker] << endl;
						operationNum++;
						*numOperations+=1;
						carryingTracker++;
						listItr.advance();
					}
				}

			   break;
			}
	}

			

	while ( (carryingTracker != 0)  ||  (dropOffCount != (carCount) ))  //main loop to run dijkstras until we arent carrying any more cars to drop off.
	{
				for (int k = 0; k < carryingTracker; k++)
				{
					if( cars[carryingCars[k]].destination == currentAddress)
					{
							dropOff (operations, operations[operationNum - 1].time, carryingCars[k], operationNum);
							cout << "dropped off:  "<<carryingCars[k] << endl;
						operationNum++;
						*numOperations+=1;
						allCars.remove(carryingCars[k]);
						carryingCars[k] = carryingCars[carryingTracker - 1];
						
						

						carryingTracker--;
						dropOffCount++;
						


					}
				}

				int start = currentAddress;
				int destination;

				if (carryingTracker == 0)
				{
					ListItr <int> listItr4 = allCars.first();
					
				 destination = cars[listItr4.retrieve()].origin;
				 cout<<"destination: "<< destination<<endl;

				}

				else{
				 destination = cars[carryingCars[carryingTracker-1]].destination;
				}

				
				

				StackLi <int> dijStack;
				//StackLi <int> dijStack2;

		dijStack = runDij(ourStations, cars, start,destination);
		//dijStack2 = dijStack;
		//int garbage2 = dijStack2.topAndPop();
		
// 		do
// 		{
// 			cout<<"move to "<<dijStack2.topAndPop()<<endl;
// 		}while (!dijStack2.isEmpty());
// cout<<"end of this dij "<<endl;



		int garbage = dijStack.topAndPop();
		
		


			while (!dijStack.isEmpty())   //loop to move the train along the path of the returned dijkstras, and check for pickups and dropoffs.
			{
				int moveTo = dijStack.topAndPop();
				int indexofMove = findIndexOfMove(ourStations,currentAddress,moveTo);

				simulatedTime += ourStations[currentAddress].distances[indexofMove];

					int ID = ourStations[currentAddress].adjacent[indexofMove];
					move (operations, simulatedTime, ID,operationNum++);
					cout << "moved to: "<<ID << endl;
					*numOperations += 1;
					currentAddress = moveTo;

				for (int j = 0; j < carryingTracker; j++)  //check for dropoff at currentAddress
				{
					if (cars[carryingCars[j]].destination == currentAddress)
					{
						dropOff (operations, operations[operationNum - 1].time, carryingCars[j], operationNum);
						cout << "dropped off:  "<<carryingCars[j] << endl;
						operationNum++;
						*numOperations+=1;
						allCars.remove(carryingCars[j]);
						carryingCars[j] = carryingCars[carryingTracker - 1];

						carryingTracker--;
						dropOffCount++;

					}
				}

				ListItr <int> listItr = stationCars[currentAddress].zeroth();
				listItr.advance();

				while (!stationCars[currentAddress].isEmpty())  //pickup all the cars we can from this station
					{
						

						if(carryingTracker != 50)
						{
							int carToRemove = listItr.retrieve();
							carryingCars[carryingTracker] = carToRemove;
							stationCars[currentAddress].remove(carToRemove);
							pickUp(operations,operations[operationNum - 1].time, carryingCars[carryingTracker], operationNum);
							cout << "pickedup:  "<<carryingCars[carryingTracker] << endl;
							operationNum++;
							*numOperations+=1;
							carryingTracker++;
							listItr.advance();
						}

						else
							break;
					}
			}
	}

	//cout <<"dropped off "<<dropOffCount<<" cars"<<endl;

} // run()

StackLi <int> Train::runDij (Station *ourStation, Car *cars, int rootNode, int destination)
{
	
	int pv[20000];
	int known[20000];
 int dv[20000];
	BinaryHeap <stationheap> minHeap (20000);
	StackLi <int> stationStack;

	for (int i = 0; i < stationCount; i++)
	{
		dv[i] = 99999999;
		known[i] = 2;
	}
	
	pv[rootNode] = -1;
	dv[rootNode] = 0;
	known[rootNode] = 1;


	
	//This loop creates new Heap Objects for all nodes adjacent to the root node, and
	//initializes their values in the respective arrays

	while (known[destination] != 1)
		{
			known[rootNode] = 1;

			for (int i =0; i < ourStation[rootNode].adjCount; i++)
			{
				if (known[ourStation[rootNode].adjacent[i]] != 1 )
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

					//check if the node's location is the destination, if so return
					for(int k=0; k < carryingTracker; k++)
					{
						if(newHeapNode.location == cars[carryingCars[k]].destination )
						{
							int pv2;
							stationStack.push(newHeapNode.location);

							pv2 = rootNode;
							do //push all pvs to stack
							{
							stationStack.push(pv2);
							pv2 = pv[pv2];
							} while(pv2 != -1);
							return stationStack; //now stack contains all IDs, including dest ID, except rootNode ID
						}
					}
				
						//check if newHeapNode.location has a pickup, if so return
					if (!stationCars[newHeapNode.location].isEmpty() && !(carryingTracker >= 50))    //check for first pickup
					{
						int pv2;
						stationStack.push(newHeapNode.location);
					
						pv2 = rootNode;
						if(pv2 != -1)
						{
							do
							{
							stationStack.push(pv2);
							pv2 = pv[pv2];
							} 			while(pv2 != -1);
						}
						return stationStack;
					}
				}
			}

			//Now that we have first rootNode node and the set of adjacent nodes, we can
			//find the shortest path of those nodes and build Dijkstra's from there
		int count = 0;
			//This loop runs until destination is found
			while (known[destination] != 1)
			{\
			//Find shortest path, set that as current node, set its known to true
				stationheap currNode = minHeap.findMin();

				//cout<<"location: "<< currNode.location<<endl;
				minHeap.deleteMin();
				known[currNode.location] = 1;
				
				count++;
				
			//This loop adds new nodes from currNode adjacents to the heap, and sets the known, pv and dv arrays of those adjacents
				for (int i = 0; i < ourStation[currNode.location].adjCount; i++)
				{

					
					if(known[ourStation[currNode.location].adjacent[i]] != 1)
					{
					//Create new heap objects from currNode adjacents and insert them to heap


						stationheap currAdj;

						if (dv[ourStation[currNode.location].adjacent[i]] > ourStation[currNode.location].distances[i] + dv[currNode.location])
						{
							currAdj.location = ourStation[currNode.location].adjacent[i];
							currAdj.distance = ourStation[currNode.location].distances[i] + dv[currNode.location];
						//stationheap currAdj(ourStation[currNode.location].adjacent[i], ourStation[currNode.location].distances[i] + dv[currNode.location]);
						//cout << "location: "<< currAdj.location <<" distance: " << currAdj.distance <<endl;

							if (known[currAdj.location] == 2)
							{
							minHeap.insert(currAdj);
							known[currAdj.location] = 0;
							}


						}  // this if statement checks if the adjacent to the current node has a path back to the rootnode that is larger than the current node we are at.

						else  // it has a path back that is shorter than the path from currnode so create currADJ with location and its original DV
						{
							currAdj.location = ourStation[currNode.location].adjacent[i];
							currAdj.distance = dv[ourStation[currNode.location].adjacent[i]];
							//stationheap currAdj(ourStation[currNode.location].adjacent[i], dv[ourStation[currNode.location].adjacent[i]]);
							//cout << "location: "<< currAdj.location <<" distance: " << currAdj.distance <<endl;
							
						}

						
						
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
							
								//then it HAS to have a dv, so check that against the addition we would add (current node's dv + distance to new node)
								if(dv[ourStation[currNode.location].adjacent[i]] > (ourStation[currNode.location].distances[i] + currNode.distance))
								{
									//dv + distance is smaller so set that as the new dv and set new pv
									dv[ourStation[currNode.location].adjacent[i]] = ourStation[currNode.location].distances[i] + currNode.distance;
									pv[ourStation[currNode.location].adjacent[i]] = currNode.location;
								}
							
							
						
							
						//RUN DEFAULT OPS**********************************************
							//else, the node wasn't visited
					
						}
					}
				}//for (iterator < adjCount)
			}//while (destination ! found)

			if (known[destination == 1])
				{return stationStack;}
	}
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
