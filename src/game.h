#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "target.h"
#include "msgqueue.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  // std::unique_ptr<Food> 
  void GenerateNewFood(const Snake* snake, std::promise<std::unique_ptr<Food>>&& prms_in);
  

 private:
  Snake snake;
  SDL_Point snakeTail;
  Food food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();

  void SnakeUpdate();
  void CheckForFood(const SDL_Point& foodLocation);

  void FoodChain(Snake* snake, bool* running);
  std::future<void> ftr_food_chain;
  Renderer* rendererHandle = nullptr;
  std::thread thread_food_generation;
  std::thread snake_update_look_for_food;
  static std::mutex mtx;

};

#endif