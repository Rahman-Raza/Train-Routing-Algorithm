#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <set>
#include <queue>

using namespace std;

#define MAX_X 3000
#define MAX_Y 3000

class StationDistance
{
public:
  short stationID;
  short stationID2;
  short distance;
  StationDistance(short ID = 0, short ID2 = 0, short dist = 0) : stationID(ID), 
    stationID2(ID2), distance(dist){}
  bool operator<(const StationDistance &rhs) const 
  {return distance < rhs.distance;}  
  //{return stationID < rhs.stationID || (stationID == rhs.stationID && distance < rhs.distance);} 
};


class Station {
public:
  short x;
  short y;
  int adjacent[10];
  int distances[10];
  int adjCount;
  Station():adjCount(0){}
};

int StationDistanceCmp (const void *ptr, const void *ptr2)
{
  return ((StationDistance*) ptr)->distance - ((StationDistance*) ptr2)->distance;
}  // StationDistanceCmp;

int find(short *sets, short ID)
{
  if(sets[ID] < 0)
    return ID;
  
  return sets[ID] = find(sets, sets[ID]);  // path compression
} //  find()


void setUnion(short *sets, short root1, short root2)
{
  if(sets[root1] > sets[root2])  // root2 is larger tree
  {
    sets[root2] += sets[root1];
    sets[root1] = root2;
  }
  else  // root1 tree is larger or equal
  {
    sets[root1] += sets[root2];
    sets[root2] = root1;
  }
}  // setUnion

void connectStations(Station *stations, int numStations)
{ // create a MST using Kruskals
  StationDistance *distances = new StationDistance[numStations * numStations];
  short *sets = new short[numStations];
  for(int i = 0; i < numStations; i++)
    sets[i] = -1;

  short root1, root2;
  
  int count = 0, count2 = 0, ID1, ID2, j;
  for(int i = 0; i < numStations - 1; i++)
    for(int j = i + 1; j < numStations; j++)
      distances[count++] = StationDistance(i, j, 
        abs(stations[i].x - stations[j].x) + abs(stations[i].y - stations[j].y));
  
  qsort(distances, count, sizeof(StationDistance), StationDistanceCmp);
  
  for(int i = 0; count2 < numStations - 1; i++)
  {
    ID1 =  distances[i].stationID;
    ID2 =  distances[i].stationID2;
    root1 = find(sets, ID1);
    root2 = find(sets, ID2);
    
    if(root1 != root2 && stations[ID2].adjCount < 10
      && stations[ID2].adjCount < 10 )
    {  
      count2++;
      setUnion(sets, root1, root2);
      stations[ID1].adjacent[stations[ID1].adjCount] = ID2;
      stations[ID1].distances[stations[ID1].adjCount++] = distances[i].distance;
      stations[ID2].adjacent[stations[ID2].adjCount] = ID1;
      stations[ID2].distances[stations[ID2].adjCount++] = distances[i].distance;
     // cout << "Added: " << ID1 << ' ' << ID2 << endl;
    }  // if doesn't create a cycle.
  }

  for(int i = 0; i < count; i++)
  {
    ID1 =  distances[i].stationID;
    ID2 =  distances[i].stationID2;
    
    if((stations[ID1].adjCount < 3 || stations[ID2].adjCount < 3)
      && stations[ID1].adjCount < 10 && stations[ID2].adjCount < 10)
    {
      for(j = 0; j < stations[ID1].adjCount; j++)
        if(stations[ID1].adjacent[j] == ID2)
          break;
      
      if(j == stations[ID1].adjCount)  // no match found
      {
        stations[ID1].adjacent[stations[ID1].adjCount] = ID2;
        stations[ID1].distances[stations[ID1].adjCount++] = distances[i].distance;
        stations[ID2].adjacent[stations[ID2].adjCount] = ID1;
        stations[ID2].distances[stations[ID2].adjCount++] = distances[i].distance;
       // cout << "Added2: " << ID1 << ' ' << ID2 << endl;
      }  // if new edge
    }  // if a station not connected to 3 and other station not connected to 10
  }  // for each edge
}  // connectStations  
  

void createStations(Station *stations, int numStations, short **map)
{
  int x, y;
  
  for(int i = 0; i < numStations; i++)
  {
    do
    {
      x = rand() % MAX_X;
      y = rand() % MAX_Y;
    } while (map[x][y] != -1);
      
    stations[i].x = x;
    stations[i].y = y;
    map[x][y] = i;
  }  
} // createStations()



void saveConnections(Station *stations, int numStations, ofstream &outf)
{
  for(int i = 0; i < numStations; i++)
  {
    outf << i << ' ' << stations[i].adjCount;
    for(int j = 0; j < stations[i].adjCount; j++)
      outf << ' ' << stations[i].adjacent[j] << ' ' << stations[i].distances[j];

    outf << endl;
  }  // for each station
} // saveConnections



void saveCars(int numStations, ofstream &outf, int cars)
{
  for(int i = 0; i < cars; i++)
  {
    int dest, start = rand() % numStations;

    while((dest = rand() % numStations) == start);

    outf << i << " " << start << " " << dest << endl;
  } // for each car
} // saveCars()

int main(int argc, char* argv[])
{
  int cars, seed, numStations, lineCount;
  short *map[MAX_X];
  for(int i = 0; i < MAX_X; i++)
  {
    map[i] = new short[MAX_Y];
    memset(map[i], -1, sizeof(short) * MAX_Y);
  }
  
  char filename[80];

  cout << "stations (5 -): ";
  cin >> numStations;
  cout << "cars: ";
  cin >> cars;
  cout << "seed: ";
  cin >> seed;

  sprintf(filename,"st-%d-%d-%d.txt",numStations, cars, seed);
  srand(seed);
  Station *stations = new Station[numStations];
  createStations(stations, numStations, map);
  connectStations(stations, numStations);
  ofstream outf(filename);
  outf << numStations << ' ' << cars << endl;
  saveConnections(stations, numStations, outf);
  outf << endl; // blank line to separate
  saveCars(numStations, outf, cars);
  outf.close();
  return 0;
}

