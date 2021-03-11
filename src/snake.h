#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include "SDL.h"
#include "target.h"

class Game;  // forward declearation. So can create a handle to Game here. Can't #include "game.h" due to recursive #include problem.

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height, Game* game_)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2),
        snakeTail({static_cast<int>(head_x), static_cast<int>(head_y)}),
        _game(game_) {}

  void Update();
  void SnakeCheckForFood(std::unique_ptr<Food>&& food, bool* running);

  void GrowBody();
  bool IsGrowing() const;
  bool SnakeCell(int x, int y) const;
  SDL_Point GetTail() const { return snakeTail; }
  
  Direction direction = Direction::kUp;
  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  SDL_Point snakeTail;
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);
  Game* _game;  // handle

  static std::mutex snakeMtx;
  bool growing{false};
  bool movedToANewCell{false};
  int grid_width;
  int grid_height;
};

#endif