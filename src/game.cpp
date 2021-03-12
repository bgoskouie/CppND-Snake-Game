#include "game.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include "SDL.h"

std::mutex Game::gameMtx;

Game::~Game() {
  std::cout << "~Game" << std::endl;
}

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height, this),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width-1)),   // random number should be inside the range (<grid_width)
      random_h(0, static_cast<int>(grid_height-1)) {
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  renderer.ClearScreenUpdate();
  rendererHandle = &renderer;
  FoodChain(&snake, &running);
  thread_score_collection = std::thread(&Game::ScoreCollector, this, &running);
    
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    if (!running) {
      std::unique_lock<std::mutex> ulock(gameMtx);
      std::cout << "Game:: snake update infinite while loop \"running\" is false" << std::endl;
      ulock.unlock();
      thread_score_collection.join();
      // std::cout << "Game:: thread_score_collection just joined" << std::endl;
      LogResultsToFile();
      // std::cout << "Game:: LogResultsToFile ends" << std::endl;
    }
    
    SnakeUpdate();
    renderer.RenderAllSnakeUpdate(snake, snake.GetTail());

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::FoodChain(Snake* snake, bool* running) {
  std::unique_lock<std::mutex> ulock(gameMtx);
  std::cout << "FoodChain up" << *running << std::endl;
  ulock.unlock();
  std::promise<std::unique_ptr<Food>> prms;
  std::future<std::unique_ptr<Food>> ftr = prms.get_future();
  GenerateNewFood(snake, std::move(prms));

  std::unique_ptr<Food> food_lcl = std::move(ftr.get());

  ftr_food_chain = std::async(std::launch::async, &Snake::SnakeCheckForFood, snake, std::move(food_lcl), running);
}


void Game::GenerateNewFood(const Snake* snake, std::promise<std::unique_ptr<Food>>&& prms_in) {
  score++;
  std::chrono::time_point<std::chrono::high_resolution_clock> captureTime = std::chrono::high_resolution_clock::now();
  char buffer[256];
  ConvertTimeToStr(buffer, captureTime);
  std::cout << "Time Gen is: " << buffer << std::endl;
  scoreLog.send(std::move(captureTime));

  std::unique_lock<std::mutex> ulock(gameMtx);
  std::cout << "GenerateNewFood starts!" << std::endl;
  ulock.unlock();

  int x, y;
  bool found = false;
  while (!found) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      found = true;
      std::unique_ptr<Food> food_local = std::make_unique<Food>(x, y);
      std::promise<std::unique_ptr<Food>> prms_local;
      std::future<std::unique_ptr<Food>> ftr_food = prms_local.get_future();
      std::async(std::launch::async, &Renderer::RenderFoodUpdate, rendererHandle, std::move(prms_local), std::move(food_local));
      std::unique_ptr<Food> food_back = ftr_food.get();
      prms_in.set_value(std::move(food_back));
    }
  }
}

void Game::SnakeUpdate() {
  if (!snake.alive) return;

  snake.Update();
}

void Game::ScoreCollector(bool* running) {
  // std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> timeVec;
  std::chrono::time_point<std::chrono::high_resolution_clock> time;
  std::unique_lock<std::mutex> ulock(gameMtx);
  std::cout << "ScoreCollector starts!" << std::endl;
  ulock.unlock();
  while (*running) {
  // Important note that an infinite loop above does not work (of course with breaking out condition!
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // setting it very high so to show if a user gets two points within 5sec, the msgqueue can hold two messages
    // ulock.lock();
    // std::cout << "ScoreCollector after sleep!" << std::endl;
    // ulock.unlock();
    while (scoreLog.getSize() > 0) {
      time = scoreLog.receive();
      timeVec.push_back(std::move(time));
    }
  }
  ulock.lock();
  std::cout << "ScoreCollector loop ends!" << std::endl;
  ulock.unlock();
}

void Game::LogResultsToFile() {
  // std::cout << "start of LogResultsToFile" << std::endl;
  // std::cout << "size of timeVec is " << timeVec.size() << std::endl;
  std::ofstream myfile;
  myfile.open("results.txt");
  myfile << "Your score is " << score << std::endl;
  myfile << "Your times of food capture are:" << std::endl;
  for (auto time : timeVec) {
    char buffer[256] = {0};
    ConvertTimeToStr(buffer, time);
    std::cout << "Timing: "  << buffer << std::endl;
    myfile << buffer << std::endl;
  }
  myfile.close();
  std::cout << "Please check the file build/results.txt for your game outputs!"  << std::endl;
}

void Game::ConvertTimeToStr(char* buffer, std::chrono::time_point<std::chrono::high_resolution_clock> time) {
  std::time_t time_c = std::chrono::high_resolution_clock::to_time_t(time);
  std::tm now_tm = *std::localtime(&time_c);
  sprintf( buffer,
           "%d-%02d-%02d %02d:%02d:%02d.%03d", 
           now_tm.tm_year + 1900,   // 1900 based
           now_tm.tm_mon + 1,       // 0 based
           now_tm.tm_mday,                      
           now_tm.tm_hour, 
           now_tm.tm_min, 
           int(now_tm.tm_sec),
           int(now_tm.tm_sec*1000) - int(now_tm.tm_sec)*1000 );
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }