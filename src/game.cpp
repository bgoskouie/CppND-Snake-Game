#include "game.h"
#include <chrono>
#include <iostream>
#include "SDL.h"

std::mutex Game::mtx;

Game::~Game() {}  // won't get called

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height, this),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)),
      food(Food(static_cast<int>(grid_width)/2, static_cast<int>(grid_height)/2)) {
  // PlaceFood();
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
  // thread_food_generation = std::thread(&Game::FoodGeneration, this, &snake, &running);
  
  
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    if (!running) {
      // thread_food_generation.join();
      // ftr_food_gen.get();
    }
    
    // Update();
    SnakeUpdate();
    // CheckForFood(food.GetLocation());
    // renderer.Render(snake, food);
    // renderer.ClearScreenUpdate();
    // renderer.RenderFoodUpdate(food);
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
  std::cout << "FoodChain up" << *running << std::endl;
  // std::cout << 
  std::promise<std::unique_ptr<Food>> prms;
  std::future<std::unique_ptr<Food>> ftr = prms.get_future();
  GenerateNewFood(snake, std::move(prms));
  // std::unique_ptr<Food> food = std::make_unique<Food>(ftr.get());
  std::unique_ptr<Food> food_lcl = std::move(ftr.get());

  // while (*running) {
    // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // std::cout << "FoodGeneration loop" << *running << std::endl;

  ftr_food_chain = std::async(std::launch::async, &Snake::SnakeCheckForFood, snake, std::move(food_lcl), running);

    // if (!running) {
    //   prms.set_value();
    //   ftr.get();
    // }
    // std::thread snake_update_look_for_food();
  // }
}


// std::unique_ptr<Food> 
void Game::GenerateNewFood(const Snake* snake, std::promise<std::unique_ptr<Food>>&& prms_in) {
  int x, y;
  bool found = false;
  // score++;
  while (!found) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      found = true;
      std::unique_ptr<Food> food_lcl2 = std::make_unique<Food>(x, y);
      std::promise<std::unique_ptr<Food>> prms_local;
      std::future<std::unique_ptr<Food>> ftr_food = prms_local.get_future();
      std::async(std::launch::async, &Renderer::RenderFoodUpdate, rendererHandle, std::move(prms_local), std::move(food_lcl2));
      std::unique_ptr<Food> food_back = ftr_food.get();
      prms_in.set_value(std::move(food_back));
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food = Food(x, y);
      std::cout << "new food at " << x << ", " << y << std::endl;
      // food.GetLocation().x = x;
      // food.GetLocation().y = y;
      return;
    }
  }
}

void Game::SnakeUpdate() {
  if (!snake.alive) return;

  snake.Update();
}

void Game::CheckForFood(const SDL_Point& foodLocation) {
  if (!snake.alive) return;

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (foodLocation.x == new_x && foodLocation.y == new_y) {
  // if (food.IsLocatedAt(new_x, new_y)) {
    score++;
    PlaceFood();
    // GenerateNewFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  // if (food.x == new_x && food.y == new_y) {
  if (food.IsLocatedAt(new_x, new_y)) {
    score++;
    PlaceFood();
    // GenerateNewFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }