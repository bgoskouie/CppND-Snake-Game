#include <iostream>
#include <string>
#include <stdexcept>
#include "target.h"

Target::Target() {}

Target::~Target() 
{
  // thrd.join();  
}

Target::Target(int x, int y, TargetType type_, SDL_Color color_)
  : location({x, y}),
    type(type_),
    color(color_)  {}

bool Target::IsLocatedAt(int x, int y) const
{
  return location.x == x && location.y == y;
}


// rule of 5 for Food class:
Food::Food(int x, int y)
  :   Target(x, y, TargetType::GOOD, {255, 255, 255, 255})  {
    std::cout << "Food constructor is called"  << std::endl;
}

Food::~Food() {}

Food::Food(const Food& other) {           // copy constructor
  std::cout << "Food copy constructor is called"  << std::endl;
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
}

Food Food::operator=(const Food& other) {   // copy assignment operator
  std::cout << "Food copy assignment operator is called"  << std::endl;
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
}

Food::Food(Food&& other) {              // move constructor
  std::cout << "Food move cosntructor is called"  << std::endl;
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
}

Food& Food::operator=(Food&& other) {   // move assignment operator
  std::cout << "Food move assignment operator is called"  << std::endl;
  if (this == &other) {
    return *this;
  }
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
  return *this;
}

