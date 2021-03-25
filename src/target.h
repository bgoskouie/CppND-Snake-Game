#ifndef TARGET_H
#define TARGET_H

#include "SDL.h"


enum class TargetType { GOOD, VERYGOOD, BAD, VERYBAD };
enum class GameColor {
  GREY,   //0x1E, 0x1E, 0x1E, 0xFF   Screen background color
  YELLOW, //0xFF, 0xCC, 0x00, 0xFF   Food
  WHITE,  //0xFF, 0xFF, 0xFF, 0xFF   Snake body
  NAVY,   //0x00, 0x7A, 0xCC, 0xFF   Snake head alive
  RED,    //0xFF, 0x00, 0x00, 0xFF   Snake head dead
};


class Color {
public:
  Color();
  Color(GameColor color_)
    : color(color_) {}
  GameColor GetColor() const {return color;}
  void SetColor(GameColor color_) {color = color_;}
  SDL_Color GetSDLColor() const;

protected:
  // has .r, .g, .b, .a fields. All are uint8_t
  GameColor color;
};


class Target : public Color
{
public:
  Target();
  Target(int x, int y, TargetType type_, GameColor color_);
  virtual ~Target();

  SDL_Point GetLocation() const {return location;}
  void SetLocation(int x, int y);
  TargetType GetType() const {return type;}
  bool IsLocatedAt(int x, int y) const;

protected:
  virtual void SetType(TargetType newType) {};

  SDL_Point location;  
  TargetType type;
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
  bool LoadTexture(SDL_Renderer *sdl_renderer);
  SDL_Texture* GetImageSurface();
  
private:
  SDL_Surface *img;
  SDL_Texture *image_texture;
  void LoadImage();
  void FreeImage();
  void FreeTexture();
};

#endif