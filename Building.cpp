//Programmer: Sadeem Khan
//Programmer ID: 1549921

#include "Building.h"
#include "Floor.h"
#include "Elevator.h"

#include <iostream>
using namespace std;

#include <cstdlib>
#include <cmath>

Floor Building::floors[] = 
{
  Floor(0, "Ground Floor", "G"), 
  Floor(100, "Second Floor", "2"),
  Floor(200, "Third Floor", "3"),
  Floor(305, "Fourth Floor", "4"),
  Floor(410, "Fifth Floor", "5"),
  Floor(510, "Sixth Floor", "6")
}; 
const int Building::FLOORS = sizeof(floors) / sizeof(Floor);

Elevator Building::elevators[] =
{
  Elevator(18, 20, 0),
  Elevator(22, 18, 1),
  Elevator(28, 12, 2),
  Elevator(34, 6, 3)
};
const int Building::ELEVATORS = sizeof(elevators) / sizeof(Elevator);

void Building::getDifferentInts(int& a, int& b)
{
  do 
  {
    a = rand() % FLOORS; // range is 0 to (FLOORS-1)
    b = rand() % FLOORS; // range is 0 to (FLOORS-1)    
  } while (a == b); // try again if they are the same
}

int Building::getPoisson(double avg)
{
  int arrivals = 0;
  double probOfnArrivals = exp(-avg); // requires cmath
  for(double randomValue = (rand() % 1000) / 1000.0; // requires cstdlib
    (randomValue -= probOfnArrivals) > 0.0; 
    probOfnArrivals *= avg / ++arrivals);
  return arrivals;
}

void Building::placeNewRiders(double arrivalsPerSecond)
{
  int n = getPoisson(arrivalsPerSecond);
  for (int i = 0; i < n; i++)
  {
    int from, to;
    getDifferentInts(from, to);
    Rider rider(from, to);
    floors[from].addRider(rider);
  }
}

bool Building::openDoorToDisembarkRider(int e)
{
  // if closed and floor button is lit...
  //...and there are riders to disembark, open door
  if (!elevators[e].isOpen())
    for (int f = 0; f < FLOORS; f++)
      if (floors[f] == elevators[e] && 
          elevators[e].panel.isLit(floors[f].label))
        {
          elevators[e].openDoorTo(f);
          return true;
        }
  return false;
}

void Building::action(double arrivalsPerSecond)
{
  placeNewRiders(arrivalsPerSecond);
  
  // one action per elevator
  for (int e = 0; e < ELEVATORS; e++)
  {
    bool actionTaken = false;

    if (openDoorToDisembarkRider(e)) continue;
    if (disembarkRider(e)) continue;
    if (boardRider(e)) continue;
    if (waitingForMoreRiders(e)) continue;
    if (doneWaitingMove(e)) continue;
    if (moveableMove(e)) continue;
    if (setIdleElevatorInMotion(e)) continue; // lab 15
    if (sendIdleElevatorToCallingFloor(e)) continue; // lab 15
    elevators[e].goIdle(); // nothing to do  
  }
}

bool Building::disembarkRider(int e) // lab 9
{
  // if open and rider to disembark, do that
  if (elevators[e].isOpen() && elevators[e].hasRiderForFloor())
  {
    elevators[e].removeRider();
    return true;
  }
  return false;
}

bool Building::boardRider(int e) // lab 9
{ 
  // if door is open and not full and rider to load, load
  if (elevators[e].isOpen() && !elevators[e].isFull())
  {
    Floor& floor = floors[elevators[e].getFloorIndex()];
    if (elevators[e].goingUp() && floor.hasUpRider())
    {
      Rider rider = floor.removeUpRider();
      elevators[e].board(rider);
      return true;
    }
    else if (elevators[e].goingDown() && floor.hasDownRider())
    {
      Rider rider = floor.removeDownRider();
      elevators[e].board(rider);
      return true;
    }
  }
  return false;
}

bool Building::waitingForMoreRiders(int e)
{
  if (elevators[e].isOpen() && !elevators[e].isIdle() && !elevators[e].timedOut()) // only if the door is open, the elevator is set to go either up or down, and the timer has not already counted  down to zero
  {
    elevators[e].tickTimer(); // "Tick" the timer
    return true;
  }
  else return false;
}

bool Building::doneWaitingMove(int e)
{
  if (!elevators[e].isOpen() || elevators[e].isIdle() || !elevators[e].timedOut()) return false;
  else
  {
    elevators[e].closeDoor();
    elevators[e].move();
  }
  return true;
}

bool Building::moveableMove(int e) 
{
  // If the door is open or if the elevator is idle, take no action and return false
  if (elevators[e].isOpen() || elevators[e].isIdle())
    return false;

  // Otherwise call Elevator::move and return whatever that call returns
  else 
    return elevators[e].move();
}

bool Building::setIdleElevatorInMotion(int e)
 {
  if (!elevators[e].isIdle()) return false;
  else
  {
    for (int i = 0; i < Building::FLOORS; i++)
     {
      if (elevators[e] != floors[i]) continue; // if the elevator's location does not match the floor's elevation exactly, skip to the next floor
      if (floors[i].panel.getFirstLit() == Floor::UP) // If that floor's first lit button is the "up" button, open the elevator door to that floor and set its direction to "up"
      {
        elevators[e].openDoorTo(i);
        elevators[e].setDirectionUp();
        return true;
      }
      else if (floors[i].panel.getFirstLit() == Floor::DOWN) // Else if its first lit button is the "down" button, open the elevator door to that floor and set its direction to "down"
      {
        elevators[e].openDoorTo(i);
        elevators[e].setDirectionDown();
        return true;
      }
    }
  }
  return false;  
}

bool Building::sendIdleElevatorToCallingFloor(int e)
 {
  if (!elevators[e].isIdle()) return false;
  else 
  {
    for (int i = 0; i < Building::FLOORS; i++)
     {
      if (!Building::floors[i].panel.areAnyLit()) continue;
      if (floors[i] > elevators[e]) 
      {
        elevators[e].setDirectionUp();
        elevators[e].move();
        return true;
      }
      else if (floors[i] < elevators[e]) 
      {
        elevators[e].setDirectionDown();
        elevators[e].move();
        return true;
      }
    }
  }
  return false; 
}