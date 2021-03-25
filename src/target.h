#ifndef TARGET_H
#define TARGET_H

#include "SDL.h"

enum class TargetType { GOOD, VERYGOOD, BAD, VERYBAD };
enum class TargetColor {
  GREY,   //0x1E, 0x1E, 0x1E, 0xFF   screen background color
  YELLOW, //0xFF, 0xCC, 0x00, 0xFF   food
  WHITE,  //0xFF, 0xFF, 0xFF, 0xFF   snake body
  NAVY,   //0x00, 0x7A, 0xCC, 0xFF   snake head alive
  RED,    //0xFF, 0x00, 0x00, 0xFF   snake head dead
};


class Target
{
public:
  Target();
  Target(int x, int y, TargetType type_, TargetColor color_);
  virtual ~Target();

  SDL_Point GetLocation() const {return location;}
  SDL_Color GetSDLColor() const;
  TargetColor GetColor() const {return color;};
  TargetType GetType() const {return type;}
  bool IsLocatedAt(int x, int y) const;

protected:
  virtual void SetType(TargetType newType) {};

  SDL_Point location;  
  TargetType type;
  TargetColor color;   // has .r, .g, .b, .a fields. All are uint8_t
};


class Food : public Target
{
public:
  Food();
  Food(int x, int y); 
  virtual ~Food();
  Food(const Food& other);
  Food operator=(const Food& other);
  Food(Food&& other);
  Food& operator=(Food&& other);

  void SetType(TargetType newType) override {type = newType;}
  bool SetImageSurface(SDL_Renderer *sdl_renderer);
  SDL_Texture* GetImageSurface();
  
private:
  SDL_Surface *img;
  SDL_Texture *image_surface;
};


#endif