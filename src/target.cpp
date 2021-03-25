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
    image_texture(NULL) {}

Food::Food(int x, int y)
  : Target(x, y, TargetType::GOOD, TargetColor::YELLOW),
    img(NULL),
    image_texture(NULL) {
    std::cout << "Food constructor is called"  << std::endl;
    LoadImage();
}

Food::~Food() {
  std::cout << "target.cpp:  Food::~Food()" << std::endl;
  FreeTexture();
  FreeImage();
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

Food::Food(Food&& other) {              // move constructor gets called upon   Food f2 = std::move(f1);
  std::cout << "Food move cosntructor is called"  << std::endl;
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
  // heap allocated member variables need to be (stolen) pointed to by the new food instance.
  img = other.img; 
  image_texture = other.image_texture;  
  other.img = NULL;
  other.image_texture = NULL;
}

Food& Food::operator=(Food&& other) {   // move assignment operator  gets called upon   Food f2; f2 = std::move(f1);
  std::cout << "Food move assignment operator is called"  << std::endl;
  if (this == &other) {
    return *this;
  }
  Target(other.GetLocation().x, other.GetLocation().y, other.GetType(), other.GetColor());
  this->FreeImage();
  // heap allocated member variables need to be (stolen) pointed to by the new food instance.
  // call is like this->img or simply img
  this->img = other.img; 
  this->image_texture = other.image_texture;  
  other.img = NULL;
  other.image_texture = NULL;
  return *this;
}

bool Food::LoadTexture(SDL_Renderer *sdl_renderer) {
  if (img) {
    if (!image_texture) {
      image_texture = SDL_CreateTextureFromSurface(sdl_renderer, img);
      std::cout << "target.cpp: image_texture is made successfully!" << std::endl;
    }
    if (!image_texture) {
      std::cerr << "target.cpp: image load error: " << SDL_GetError() << std::endl;
      return false;
    }
    return true;
  }
  return false;
}

void Food::LoadImage() {
  img = SDL_LoadBMP("image/sdllogo.bmp");
  if (!img) {
    std::cerr << "target.cpp: image load error: " << SDL_GetError() << std::endl;
  }
}

void Food::FreeImage() {
  if (!img) {
    SDL_FreeSurface(img);
  }
}

void Food::FreeTexture() {
  if (!image_texture) {
    SDL_DestroyTexture(image_texture);
  }
}

SDL_Texture* Food::GetImageSurface() {
  return image_texture;
}