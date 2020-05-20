//Programmer: Sadeem Khan
//Programmer ID: 1549921

#include "Elevator.h"
#include "Panel.h"
#include "Building.h"

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#include <cstdlib>

Elevator::Elevator(unsigned int capacity, int speed, int start)
:speed(speed), CAPACITY(capacity), direction(IDLE), atFloorIndex(-1), location(Building::floors[start]), timer(0)
{
  for (int i = 0; i < Building::FLOORS; i++)
  {
    panel.addButton(Building::floors[i].label);
  }
}

ostream& operator<<(ostream& out, const Elevator& e) 
{
  out << "Elevator at";
  out.width(6);
  out << e.location;

  out.width(13);
  if(e.direction == e.UP)
  {
    out << " going UP ";
  }

  else if(e.direction == e.DOWN)
  {
    out << " going DOWN ";
  }

  else
  {
    out << " idle ";
  }

  out.width(4);
  out << e.riders.size() << " " << "riders";

  if (e.atFloorIndex != -1) out << " door is OPEN|" << e.timer << " ";

  out << e.panel;  
  return out;
}

void Elevator::openDoorTo(int f)
{
  panel.clear(Building::floors[f].label); // clear the host elevator's panel button for the referenced floor (so its light goes out) -- call a Panel setter...
  atFloorIndex = f; // set atFloorIndex to the referenced floor's index
  resetTimer(); // reset the host elevator's count-down timer
}

void Elevator::board(const Rider& r)
{
  riders.push_back(r); // .push_back the Rider to the host elevator's vector of Riders
  panel.press(Building::floors[r.to].label); // access the label for the Rider's destination floor and press the corresponding button on the panel
  if (r.goingUp == true) direction = UP; // if the Rider is going up, set the host elevator's direction to up,
  else direction = DOWN; // otherwise set the direction down
  resetTimer(); // reset the host elevator's count-down timer
}

bool Elevator::hasRiderForFloor() const
{
  if (atFloorIndex == -1) return false; // if the door is not open, return false -- nothing to do 

  for(int i = 0; i < riders.size(); i++) // traverse the host elevator's vector of riders
    if(riders[i].to == atFloorIndex) return true; // if a rider's destination equals the elevator's current floor return true

  return false; //if the end of the traversal loop is reached, return false
}

void Elevator::removeRider()
{
  vector<Rider>:: iterator it;
  for(it = riders.begin(); it != riders.end(); it++) // traverse the host elevator's vector of riders
  {
    if(it->to == atFloorIndex) // if a rider's destination equals the elevator's current floor 
    {
      this->panel.clear(Building::floors[it->to].label);
      it = this->riders.erase(it);
      this->resetTimer();
      break;
    }
  }
}

bool Elevator::isIdle() const
{
  return (direction == IDLE);
}

bool Elevator::closeDoor()
{
  if (atFloorIndex == -1) return false;
  else
  {
    // If the elevator is set to go up, and there are no other up-riders on the floor to which its door is open, clear that floor's "up" button on its panel.
    if (!Building::floors[atFloorIndex].hasUpRider() && goingUp()) Building::floors[atFloorIndex].panel.clear(Building::floors[atFloorIndex].UP);
    if (!Building::floors[atFloorIndex].hasDownRider() && goingDown()) Building::floors[atFloorIndex].panel.clear(Building::floors[atFloorIndex].DOWN);
    atFloorIndex = -1; // Close the door
  }
return true;
}

bool Elevator::move()
{
  // For up-moving Elevators
  if (direction == UP)
  {
    for (int i = 0; i < Building::FLOORS; i++) // Cycle through each floor.
    {
      if (Building::floors[i].elevation < location || ((location + speed) < Building::floors[i].elevation)) continue;
      if (panel.isLit(Building::floors[i].label) || Building::floors[i].panel.isLit(Floor::UP))
      {
        if (location != Building::floors[i].elevation)
        {
          location = Building::floors[i].elevation;
          openDoorTo(i);
          return true;
        }
      }
    }

    if (location == Building::floors[Building::FLOORS - 1].elevation) return false;
    else
    {
      location = location + speed;
      return true;
    }
  }

  // For down-moving Elevators
  if (direction == DOWN)
  {
    for (int i = 0; i < Building::FLOORS; i++)
    {
      if (Building::floors[i].elevation > location || ((location - speed) > Building::floors[i].elevation)) continue;
      if (panel.isLit(Building::floors[i].label) || Building::floors[i].panel.isLit(Floor::DOWN))
      {
        if (location != Building::floors[i].elevation)
        {
          location = Building::floors[i].elevation;
          openDoorTo(i);
          return true;
        }
      }
    }

    if (location == Building::floors[0].elevation) return false;
    else
    {
      location = location - speed;
      return true;
    }
  }
  return false;
}