//Programmer: Sadeem Khan
//Programmer ID: 1549921

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>
using namespace std;

#include "Rider.h"
#include "Floor.h"
#include "Panel.h"

class Elevator
{
  static const int IDLE = 0; 
  static const int UP = 1;
  static const int DOWN = -1;

  // elevator metrics
  int location; // inches above ground level
  int direction; // equal to IDLE, UP, DOWN
  int atFloorIndex; // -1 if door is closed
  int timer;

  vector<Rider> riders;
  
  public:
  Elevator(unsigned int, int, int);
  operator int() const {return location;}
  const int speed; // inches per second, up or down
  const unsigned int CAPACITY;


  // count-down timer before closing door
  void resetTimer() {timer = 3;}
  void tickTimer() {timer--;}
  bool timedOut() const {return timer == 0;}

  bool isOpen() const {return atFloorIndex >= 0;}
  void openDoorTo(int); // parameter is index in Building::floors array
  void board(const Rider&); // adds a Rider to the Elevator
  bool hasRiderForFloor() const;
  void removeRider();
  bool isFull() const {return CAPACITY == riders.size( );}
  bool goingUp() const {return direction == UP;}
  bool goingDown() const {return direction == DOWN;}
  int getFloorIndex() const {return atFloorIndex;}
  unsigned int getNumberOfRiders() const {return riders.size();}
  bool isIdle() const;
  bool closeDoor();
  bool move();
  void goIdle() {direction = IDLE;}
  void setDirectionUp() {direction = UP;} // Let them set the elevator's direction to up or down, respectively.
  void setDirectionDown() {direction = DOWN;} // Let them set the elevator's direction to up or down, respectively.

  Panel panel;
  
  // friend function
  friend ostream& operator<<(ostream&, const Elevator&); 
};
#endif