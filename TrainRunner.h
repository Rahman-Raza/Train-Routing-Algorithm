#ifndef TRAINRUNNER_H
#define	TRAINRUNNER_H

class Operation
{
public:
  int time;
  char operation; // 'M', 'P', or 'D'
  short ID;  // CarID or StationID
  Operation(int t = 0, char o = 'M', short ID = 0) : time(t), operation(o),
    ID(ID) {}
}; // class Operation


class Station {
public:
  int adjacent[10];
  int distances[10];
  int adjCount;
  Station():adjCount(0){}
};

class Car{
public:
  int origin;
  int destination;
  Car(int orig = -1, int dest = -1) : origin(orig), destination(dest) {}
}; // class Car

#endif	/* TRAINRUNNER_H */

