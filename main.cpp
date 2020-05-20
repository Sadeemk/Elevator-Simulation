//Programmer: Sadeem Khan
//Programmer ID: 1549921

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <cstdlib>
#include <ctime>

#include "Rider.h"
#include "Building.h"
#include "Floor.h"
#include "Panel.h"

int main()
{
  // Identification
  cout << "Programmer: Sadeem Khan\n";
  cout << "Programmer's ID: 1549921\n";
  cout << "File: " << __FILE__ << endl;

  srand(time(0)); rand();

  for (int i = 0;; i++)
  { 
    cout << "-----------------------------------------------\n";
    cout << "-- Time " << i << " ---------------------------\n";
    for (int elevator = 0; elevator < Building::ELEVATORS; elevator++)
       cout << Building::elevators[elevator] << endl;

    for (int floor = 0; floor < Building::FLOORS; floor++)
       cout << Building::floors[floor] << endl;

    double myArrivalRate = 2.0; // you choose!
    Building::action(i < 300 ? myArrivalRate : 0);
 
    cout << "Press ENTER to continue... ";
    cin.get();
  }
}