#include "snake.h"
#include <cmath>
#include <iostream>
#include "game.h"  // needed for Game::ReplaceFood

std::mutex Snake::snakeMtx;

void SnakeHead::UpdateHead(float speed, SnakeDirection direction, int max_width, int max_height) {
  switch (direction) {
    case SnakeDirection::kUp:
      head_y -= speed;
      break;

    case SnakeDirection::kDown:
      head_y += speed;
      break;

    case SnakeDirection::kLeft:
      head_x -= speed;
      break;

    case SnakeDirection::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + max_width, max_width);
  head_y = fmod(head_y + max_height, max_height);
}

void Snake::Update() {
  // First capture the head's cell before updating.
  SDL_Point prev_cell{
      static_cast<int>(head.GetX()),
      static_cast<int>(head.GetY())};  
  head.UpdateHead(speed, direction, grid_width, grid_height);
  // Capture the head's cell after updating.
  SDL_Point current_cell{
      static_cast<int>(head.GetX()),
      static_cast<int>(head.GetY())};

  // Update all of the body vector items if the snake head has moved 
  // to a new cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
    movedToANewCell = true;
  } else {
    movedToANewCell = false;
  }
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.points.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    snakeTail = body.points.at(0);
    body.points.erase(body.points.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body.points) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
      head.SetColor(GameColor::RED);
    }
  }
}

void Snake::GrowBody() { growing = true; }

bool Snake::IsGrowing() const { return growing; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) const {
  if (x == static_cast<int>(head.GetX()) && 
      y == static_cast<int>(head.GetY())) {
    return true;
  }
  for (auto const &item : body.points) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

void Snake::SnakeCheckForFood(std::unique_ptr<Food>&& food, bool* running) {
  std::unique_lock<std::mutex> ulock(snakeMtx);
  std::cout << "SnakeCheckForFood: up = " << *running << std::endl;
  ulock.unlock();
  while (*running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // std::cout << "FoodGeneration loop" << *running << std::endl;
    if (!alive) return;

    int new_x = static_cast<int>(head.GetX());
    int new_y = static_cast<int>(head.GetY());

    if (food->IsLocatedAt(new_x, new_y)) {
      ulock.lock();
      std::cout << "SnakeCheckForFood: food is found" << std::endl;
      ulock.unlock();
      std::promise<std::unique_ptr<Food>> prms;
      std::future<std::unique_ptr<Food>> ftr = prms.get_future();

      std::async(std::launch::async, &Game::ReplaceFood, _game, 
                          this, std::move(prms), std::move(food));
      food = std::move(ftr.get());  // calls the Food's move assignment operator
      std::cout << "SnakeCheckForFood: food is replaced and back!" << std::endl;
      // Grow snake and increase speed.
      GrowBody();
      speed += 0.02;
    }
  }
}
