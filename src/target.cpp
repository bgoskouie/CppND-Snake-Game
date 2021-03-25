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

void Target::SetLocation(int x, int y) {
  location.x = x;
  location.y = y;
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
Food::Food()
  : img(NULL),
    image_surface(NULL) {}

Food::Food(int x, int y)
  : Target(x, y, TargetType::GOOD, TargetColor::YELLOW),
    img(SDL_LoadBMP("image/sdllogo.bmp")),
    image_surface(NULL) {

    if (!img) {
      std::cerr << "target.cpp: image load error: " << SDL_GetError() << std::endl;
    }
    std::cout << "Food constructor is called"  << std::endl;
}

Food::~Food() {
  std::cout << "target.cpp:  Food::~Food()" << std::endl;
  if (!image_surface) {
    SDL_DestroyTexture(image_surface);
  }
  if (!img) {
    SDL_FreeSurface(img);
  }
}

Food::Food(const Food& other) {           // copy constructor
  std::cout << "Food copy constructor is called"  << std::endl;
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
}

Food Food::operator=(const Food& other) {   // copy assignment operator
  std::cout << "Food copy assignment operator is called"  << std::endl;
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
  return *this;
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

bool Food::SetImageSurface(SDL_Renderer *sdl_renderer) {
  if (img) {
    if (!image_surface) {
      image_surface = SDL_CreateTextureFromSurface(sdl_renderer, img);
      std::cout << "target.cpp: image_surface is made successfully!" << std::endl;
    }
    if (!image_surface) {
      std::cerr << "target.cpp: image load error: " << SDL_GetError() << std::endl;
      return false;
    }
    return true;
  }
  return false;
}

SDL_Texture* Food::GetImageSurface() {
  return image_surface;
}