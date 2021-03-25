#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include "SDL.h"
#include "target.h"

// forward declearation. So we can create a handle to the Game here.
// Can't #include "game.h" due to recursive #include problem.
class Game;


enum class SnakeDirection { kUp, kDown, kLeft, kRight };

class SnakeBody : public Color
{
public:
  SnakeBody()
    : Color(GameColor::WHITE) {};
  std::vector<SDL_Point> points;
private:
};


class SnakeHead : public Color
{
public:
  SnakeHead() {}
  SnakeHead(float x_, float y_, GameColor color_)
    : head_x(x_),
      head_y(y_),
      Color(color_) {};
  float GetX() const {return head_x;}
  float GetY() const {return head_y;}
  void UpdateHead(float speed, SnakeDirection direction, int max_width, int max_height);

private:
  float head_x, head_y;
  void SetHeadColor(GameColor color_in) {color = color_in;}
};


class Snake
{
 public:
  Snake(int grid_width, int grid_height, Game* game_)
      : grid_width(grid_width),
        grid_height(grid_height),
        // a live snake head is Navy!
        head(SnakeHead(grid_width / 2, grid_height / 2, GameColor::NAVY)),
        snakeTail({static_cast<int>(head.GetX()), static_cast<int>(head.GetY())}),
        _game(game_) {}

  void Update();
  void SnakeCheckForFood(std::unique_ptr<Food>&& food, bool* running);

  void GrowBody();
  bool IsGrowing() const;
  bool SnakeCell(int head_x, int head_y) const;
  SDL_Point GetTail() const { return snakeTail; }
  
  SnakeDirection direction = SnakeDirection::kUp;
  float speed{0.1f};
  int size{1};
  bool alive{true};
  SnakeHead head;
  SnakeBody body;

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