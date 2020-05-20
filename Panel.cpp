//Programmer: Sadeem Khan
//Programmer ID: 1549921

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "Panel.h"

int Panel::Button::totalNumberPressed = 0;

ostream& operator<<(ostream& out, const Panel& p)
{
  for(unsigned int i = 0; i < p.buttons.size(); i++)
  {
    if(p.buttons[i].lit) out << "[" << p.buttons[i].label << "]";  
  }

  return out;
}

void Panel::addButton(string l)
{
  Button b = {l, false};
  buttons.push_back(b);
}

void Panel::press(string l)
{
  for(int i = 0; i < buttons.size(); i++)
  {
    if(!buttons[i].lit && buttons[i].label == l)
    {
      buttons[i].lit = true;
      buttons[i].sequenceNumber = Button::totalNumberPressed++;
    }
  }
}

void Panel::clear(string x)
{
  for(int i = 0; i < buttons.size(); i++)
  {
    if(buttons[i].label == x)
    {
      buttons[i].lit = false;
    }
  }
}

string Panel::getFirstLit() const
{
  int f = -1;
  for (unsigned int i = 0; i < buttons.size(); i++)
  {
    if (buttons[i].lit)
    {
      if (f == -1) f = i;
      if (buttons[f].sequenceNumber > buttons[i].sequenceNumber) f = i;
    }
  }
  return f != -1 ? buttons[f].label : "";
}

bool Panel::isLit(string x) const
{
  for (unsigned int i = 0; i < buttons.size(); i++)
  {
    if(buttons[i].lit && buttons[i].label == x) return true;
  }
  return false;
}

bool Panel::areAnyLit() const
{
  for (unsigned int i = 0; i < buttons.size(); i++)
  {
    if(buttons[i].lit) return true;
  }
  return false;
}