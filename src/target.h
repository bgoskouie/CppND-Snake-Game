#ifndef TARGET_H
#define TARGET_H

#include <mutex>
#include <thread>
#include <future>
#include "SDL.h"
//#include "snake.h"

enum class TargetType { GOOD, VERYGOOD, BAD, VERYBAD };

class Target
{
public:
  Target();
  Target(int x, int y, TargetType type_, SDL_Color color_);
  virtual ~Target();

  SDL_Point GetLocation() const {return location;}
  SDL_Color GetColor() const {return color;}
  TargetType GetType() const {return type;}
  bool IsLocatedAt(int x, int y) const;
  void SetType(TargetType newType) {type = newType;}
  //virtual void Place(TargetType&& type_, const Snake* snake) {};

protected:
  SDL_Point location;  
  TargetType type;
  SDL_Color color;   // has .r, .g, .b, .a fields. All are uint8_t


};


class Food : public Target
{
public:
  Food(int x, int y); 
  virtual ~Food();
  Food(const Food& other);
  Food operator=(const Food& other);
  Food(Food&& other);
  Food& operator=(Food&& other);



private:
  bool placed;
};


#endif