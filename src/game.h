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
  void ReplaceFood(const Snake* snake, std::promise<std::unique_ptr<Food>>&& prms_in, std::unique_ptr<Food>&& food);

  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point snakeTail;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void SnakeUpdate();

  void FoodChain(Snake* snake, bool* running);
  void FindNewLocationForFood(const Snake* snake, int& x, int& y);
  std::future<void> ftr_food_chain;
  Renderer* rendererHandle = nullptr;
  std::thread thread_score_collection;
  void ScoreCollector(bool* running);
  MessageQueue<std::chrono::time_point<std::chrono::high_resolution_clock>> scoreLog;
  std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> timeVec;
  void LogResultsToFile();
  void ConvertTimeToStr(char* buffer, std::chrono::time_point<std::chrono::high_resolution_clock> time);
  static std::mutex gameMtx;
};

#endif