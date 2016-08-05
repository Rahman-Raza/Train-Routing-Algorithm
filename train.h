#ifndef TRAIN_H
#define	TRAIN_H
#include "TrainRunner.h"
#include "BinaryHeap.h"
#include "LinkedList.h"
#include "StackLi.h"

class Train
{

public:
	Station *ourStations;
	Car *ourCars;
	int stationCount;
	int carCount;
	List<int> stationCars[20000];
	List<int> stationDestinations[20000];
	List<int> allCars;
	int operationNum;
	int simulatedTime;
	int carryingCars[50];
	int carryingTracker;
	int dropOffCount;
	int rootNode;
	int currentAddress;

	int findIndexOfMove(Station *ourStations, int station, int adjacent);

	StackLi <int> runDij (Station *ourStation,Car *cars, int rootNode, int destination);
	void pickUp (Operation *operations,  int simTime, int ID, int operationNum);
	void dropOff (Operation *operations,  int simTime, int ID, int operationNum);
	void move (Operation *operations,  int simTime, int ID, int operationNum);

	class DIJ
	{
	public:
	int currentStation;
	int previousStation[20000];
	bool known[20000];
	int distance[20000];
	DIJ(int station);
	};

	class stationheap
	{
	public:
		unsigned int distance;
		int location;
		
		stationheap( int loc, unsigned int dist):distance(dist), location(loc) {};
		stationheap(){
			distance = -1;
			location = -1;
		}

		 bool operator<( const stationheap & rhs ) const {
	       return distance < rhs.distance;
	    }

	    bool operator>( const stationheap & rhs) const {
	       return distance > rhs.distance;
	    }
	};

  Train(Station *stations, int numStations, int numCars);
  void run(Car *cars, Operation *operations, int *numOperations);
}; // class train 

#endif	// TRAIN_H

