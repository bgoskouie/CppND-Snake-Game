#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "target.h"
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void UpdateWindowTitle(int score, int fps);
  void UpdateScreen();
  void ClearScreenUpdate();
  void ClearScreen();
  std::unique_ptr<Food> RenderFood(std::unique_ptr<Food>&& food);
  void RenderFoodUpdate(std::promise<std::unique_ptr<Food>>&& prms, std::unique_ptr<Food>&& food);  
  void RenderSnakesBody(Snake const& snake);
  void RenderSnakesHead(Snake const& snake);
  void ClearLastPoint(SDL_Point const &snakeTail);
  void RenderAllSnakeUpdate(Snake const& snake, SDL_Point const &snakeTail);


 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif