//Programmer: Sadeem Khan
//Programmer ID: 1549921

#ifndef Rider_h
#define Rider_h

struct Rider
{
  const int to;
  const int from;
  const bool goingUp;
  const bool goingDown;
  Rider(int, int); 
  Rider& operator=(const Rider&);
};
#endif