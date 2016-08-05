#include <fstream>
#include <iostream>
#include "CPUTimer.h"
#include "train.h"
#include "TrainRunner.h"

#define MAX_LOAD 50
using namespace std;

void readFile(const char *filename, Station **st, Station **st2, 
  Car **car, Car **car2, int *numStations, int *numCars)
{
  int ID, adjCount;
  Station *stations, *stations2;
  Car *cars, *cars2;
  ifstream inf(filename);
  inf >> *numStations >> *numCars;
  *st = stations = new Station[*numStations];
  *st2 = stations2 = new Station[*numStations];
  *car = cars = new Car[*numCars];
  *car2 = cars2 = new Car[*numCars];
  
  for(int i = 0; i < *numStations; i++)
  {
    inf >> ID >> adjCount;
    stations2[i].adjCount = stations[i].adjCount = adjCount;
    
    for(int j = 0; j < adjCount; j++)
    {
      inf >> stations[i].adjacent[j] >> stations[i].distances[j];
      //stations2[i].adjacent[j] = stations[i].adjacent[j];
      //stations2[i].distances[j] = stations[i].distances[j];
    }  // for j
    
    stations2[i] = stations[i];
  }  // for each station
  
  inf.ignore(1000, '\n');
  inf.ignore(1000, '\n');

  for(int i = 0; i < *numCars; i++)
  {
    inf >> ID >> cars[i].origin >> cars[i].destination;
    cars2[i] = cars[i];
  }  // for each car
  
  inf.close();
} // readFile()


int checkOperations(Operation *operations, int numOperations, Car *cars, 
    Station *stations, int numCars, int numStations)
{
  int time = 0, time2, delivered = 0, carCount = 0;
  int currentStation = 0, j, carIDs[MAX_LOAD];
 
  
  for(int i = 0; i < numOperations; i++)
  {
    time2 = operations[i].time;
    
    if(time2 < time)
      cout << "Time went backwards at time " << time2 << endl;
    
    switch (operations[i].operation)
    {
      case 'M' :
        for(j = 0; j < stations[currentStation].adjCount; j++)
          if(stations[currentStation].adjacent[j] == operations[i].ID)
            break;

        if(j == stations[currentStation].adjCount)
          cout << "At time " << time2 << ", no connection between "
            << currentStation << " and " << operations[i].ID << endl;
        else
          if(time2 - time != stations[currentStation].distances[j])
            cout << "At time " << time2 << ", arrived from station #" 
              << currentStation << " at station# " << operations[i].ID 
              << " in " << time2 - time << " units instead of " 
              << stations[currentStation].distances[j] << " units\n";
        
        currentStation = operations[i].ID;
        break;
      case 'P' :
    
        if(carCount == MAX_LOAD)
          cout << "At time " << time2 << ", train has more than " << MAX_LOAD << " cars\n";
        else
        {
          if(cars[operations[i].ID].origin != currentStation)
            cout << "At time " << time2 << ", " << operations[i].ID
              << " does not originate at " << currentStation << endl;
          carIDs[carCount++] = operations[i].ID;
        }

        if(cars[operations[i].ID].origin != -1)
          cars[operations[i].ID].origin = -1;
        else
          cout << "At time " << time << ", duplicate pickup of car #" 
            << operations[i].ID << endl;
        break;
      case 'D' :
        for(j = 0; j < carCount; j++)
          if(carIDs[j] == operations[i].ID)
            break;

        if(j == carCount)
          cout << "At time " << time << ", car#" << operations[i].ID 
            << " was not on the train.\n";
        else
          carIDs[j] = carIDs[--carCount];

        if(cars[operations[i].ID].destination != currentStation)
          cout << "At time " << time << ", car #" << operations[i].ID 
            << " does not have station #" << currentStation 
            << " as its destination.\n";

        if(cars[operations[i].ID].destination != -1)
        {
          cars[operations[i].ID].destination = -1;
          delivered++;
        }
        else
          cout << "At time " << time << ", duplicate delivery of car# " 
            << operations[i].ID << endl;
        break;
      default: 
        cout << "Operation " << operations[i].operation << " unknown.\n";
        break;
    } // switch
    
    time = time2;
  } // while more to read

  if(delivered != numCars)
    cout << "Only " << delivered << " cars delivered.\n";

  return time;
}  // checkOperations

int main(int argc, char* argv[])
{
  CPUTimer ct;
  Station *stations, *stations2;
  Car *cars, *cars2;
  Operation *operations = new Operation[10000000]; 
  int numCars, numStations, numOperations;
  readFile(argv[1], &stations, &stations2, &cars, &cars2, &numStations, &numCars); 
  ct.reset();
  Train *train = new Train(stations, numStations, numCars);
  delete [] stations;
  train->run(cars, operations, &numOperations);
  double time = ct.cur_CPUTime();
  int simulatedTime = checkOperations(operations, numOperations, cars2, 
    stations2, numCars, numStations);
  cout << "CPU Time: " << time << " Simulated time: " << simulatedTime <<  " numOperations: " << numOperations << endl; 
  return 0;
}  // main()

