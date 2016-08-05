// Author: Sean Davis

#include "TrainRunner.h"
#include "train.h"
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
	start = 0;
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

	runDij(start, cars, operations, numOperations);
	return;


  
} // run()



void Train::runDij (int start, Car *cars, Operation *operations, int *numOperations)
{
	//stationCars[4].remove(1);
	int nextAddress;
	int nextDistance;
	int sortedAdjList[10];
	//int start is our start vertex

	BinaryHeap <int> minHeap(10);

	//This for loop runs for as many adjancent paths there are to the start vertex
	//It inserts all of the adjacent nodes into the minHeap
	for (int i = 0; i < ourStations[start].adjCount; i++)
	{
		minHeap.insert(ourStations[start].distances[i]);
	}


	//Runs for each adjacent path
	for (int i = 0; i < ourStations[start].adjCount; i++)
	{
		nextDistance = minHeap.findMin(); //nextAddress is the distance
		minHeap.deleteMin();
		
		//find the index where this nextDistance is

		for(i = 0; i < 10; i++)  //this for loop finds the index of the next movement vertex based off the lowest distance.
		{
			if(ourStations[start].distances[i] == nextDistance)
			{
				nextAddress = ourStations[start].adjacent[i];
				break;
			}
		}

		ListItr <int> listItr = stationCars[nextAddress].zeroth(); 

		if (carryingTracker !=50)  //if there is enough space for us to pick up a car 
		{

			if (!stationCars[nextAddress].isEmpty()) //this means there is at least one car at that station breh so we move to it and pick up a car.  
			{

			
				
				simulatedTime += nextDistance;
				
				move(operations, simulatedTime, nextAddress, operationNum);
				operationNum++;
				
				*numOperations+=1;


				while (!stationCars[nextAddress].isEmpty())   //while the next address were going to has cars
				{
					//we will use this listItr to check the cars that this array contains
					ListItr <int> listItr = stationCars[nextAddress].zeroth(); 
					

					if (carryingTracker != 50)
					{
					
						listItr.advance();  //move away from zero garbage position
						if (!listItr.isPastEnd())  //check if still anything in the list
						{
							//get the car at the next spot after zero garbage
							int carToRemove = listItr.retrieve();
							//Add this to our cars we are carrying
							carryingCars[carryingTracker] = carToRemove;
							//stationCars[4].remove(1);

							//remove it from the stationCars, indicating that we have picked it up
							stationCars[nextAddress].remove(carToRemove);
							
							pickUp(operations, operations[operationNum - 1].time, carryingCars[carryingTracker], operationNum);
							operationNum++;
					
							
							 *numOperations+=1;
							carryingTracker++;  //increment carrying array
						}
					}
					else  
						break;
				} //while loop

				for (int j = 0; j < carryingTracker; j++)   //checking to see if any of the cars wer are carring can be dropped off
				{
					if (cars[carryingCars[j]].destination == nextAddress)  //if one of the cars we are carrying can be dropped of at nextAddress
					{

						dropOff(operations, operations[operationNum - 1].time, carryingCars[j], operationNum);
						operationNum++;
					    
					    *numOperations+=1;

						carryingCars[j] = carryingCars[carryingTracker - 1];
					
						carryingTracker--;
						dropOffCount++;

					}

				}
				//Have moved now, should call run again, so reset start and call runDij again
				start = nextAddress;
				runDij(start, cars, operations, numOperations);
				return;
			}  //if statement for if it has cars

		} //if space to pick up cars 



		for (int j = 0; j < carryingTracker; j++)   //checking to see if any of the cars wer are carring can be dropped off
		{
			if (cars[carryingCars[j]].destination == nextAddress)  //if one of the cars we are carrying can be dropped of at nextAddress
			{
				//Found at least one drop off so move there
				simulatedTime += nextDistance;
				move(operations, simulatedTime, nextAddress, operationNum);
				operationNum++;
			    *numOperations+=1;

				
				//starting with current index to rest, drop off everything
				for (int k = j; k <= carryingTracker; k++)
				{
					if (cars[carryingCars[k]].destination == nextAddress)  //if one of the cars we are carrying can be dropped of at nextAddress
					{
						dropOff(operations, operations[operationNum - 1].time, carryingCars[k], operationNum);
						
						operationNum++;
				
						 *numOperations+=1;


						carryingCars[k] = carryingCars[carryingTracker - 1];
				
						carryingTracker--;
						dropOffCount++;
					}
				}//finished dropping off everything
				
				if (dropOffCount == carCount)
				{
					return;
				}
				else
				{
					start = nextAddress;
					runDij(start, cars, operations, numOperations);
					return;
				}
			}
		}
	} //for loop for this vertex
	
} //runDij()

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
