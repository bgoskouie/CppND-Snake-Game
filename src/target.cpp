#include <iostream>
#include <string>
#include <stdexcept>
#include "target.h"

Target::Target() {}

Target::~Target() 
{
  // thrd.join();  
}

Target::Target(int x, int y, TargetType type_, TargetColor color_)
  : location({x, y}),
    type(type_),
    color(color_)  {}

bool Target::IsLocatedAt(int x, int y) const
{
  return location.x == x && location.y == y;
}

SDL_Color Target::GetSDLColor() const {
  switch(color) {
    case TargetColor::GREY:
      return {0x1E, 0x1E, 0x1E, 0xFF};
    case TargetColor::YELLOW:
      return {0xFF, 0xCC, 0x00, 0xFF};
    case TargetColor::WHITE:
      return {0xFF, 0xFF, 0xFF, 0xFF};
    case TargetColor::NAVY:
      return {0x00, 0x7A, 0xCC, 0xFF};
    case TargetColor::RED:
      return {0xFF, 0x00, 0x00, 0xFF};
    default:
      return {0x00, 0x00, 0x00, 0xFF};
  }
}


// rule of 5 for Food class:
Food::Food() {}

Food::Food(int x, int y)
  :   Target(x, y, TargetType::GOOD, TargetColor::YELLOW)  {
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
