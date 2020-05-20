//Programmer: Sadeem Khan
//Programmer ID: 1549921

#include "Floor.h"
#include "Rider.h"
#include "Panel.h"
#include "Building.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

#include <climits>

const char* const Floor::UP = "Up";
const char* const Floor::DOWN = "Down";
int Floor::TOP = INT_MIN; // defined in climits
int Floor::BOTTOM = INT_MAX; // also in climits

Floor::Floor(const int elevation, const char* const name, const char* const label)
:name(name), elevation(elevation), label(label)
{
  panel.addButton(UP);
  panel.addButton(DOWN);
  if (TOP < elevation) TOP = elevation;
  if (elevation < BOTTOM) BOTTOM = elevation;
}

ostream& operator<<(ostream& out, const Floor& floor)
{
  out << setw(2) << floor.label;
  out << setw(15) << floor.name << " at";
  out << setw(8) << floor.elevation << "\" Up/Down:  ";
  out << setw(2) << floor.upRiders.size() << "/" << floor.downRiders.size(); 
  out << setw(12) << "Buttons: " << floor.panel;
  return out;
}

void Floor::addRider(const Rider& r)
{
  if(r.goingUp)
  {
    upRiders.push(r);
    panel.press(UP);
  }

  else
  {
    downRiders.push(r);
    panel.press(DOWN);
  }
}

Rider Floor::removeUpRider()
{
  if(upRiders.empty()) throw "EMPTY!"; // throw an exception if the queue is empty
  Rider r = upRiders.front(); // make a local copy of the rider at the front of the up-rider queue
  upRiders.pop(); // pop the rider at the front of the up-rider queue
  return r; // return the local copy
}

Rider Floor::removeDownRider()
{
  if(downRiders.empty()) throw "EMPTY!"; // throw an exception if the queue is empty
  Rider r = downRiders.front(); // make a local copy of the rider at the front of the down-rider queue
  downRiders.pop(); // pop the rider at the front of the down-rider queue
  return r; // return the local copy
}