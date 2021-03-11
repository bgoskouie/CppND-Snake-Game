#include "snake.h"
#include <cmath>
#include <iostream>
#include "game.h" // needed for Game::GenerateNewFood

std::mutex Snake::snakeMtx;

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
    movedToANewCell = true;
  } else {
    movedToANewCell = false;
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    snakeTail = body.at(0);
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

bool Snake::IsGrowing() const { return growing; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) const {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

void Snake::SnakeCheckForFood(std::unique_ptr<Food>&& food, bool* running) {
  std::unique_lock<std::mutex> ulock(snakeMtx);
  std::cout << "SnakeCheckForFood up" << *running << std::endl;
  ulock.unlock();
  while (*running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // std::cout << "FoodGeneration loop" << *running << std::endl;
    if (!alive) return;

    int new_x = static_cast<int>(head_x);
    int new_y = static_cast<int>(head_y);

    if (food->IsLocatedAt(new_x, new_y)) {
      ulock.lock();
      std::cout << "Snake::snakeCheckForFood food is found" << std::endl;
      ulock.unlock();
      std::promise<std::unique_ptr<Food>> prms;
      std::future<std::unique_ptr<Food>> ftr = prms.get_future();

      std::async(std::launch::async, &Game::GenerateNewFood, _game, this, std::move(prms));
      food = std::move(ftr.get());  // calls the Food's move assignment operator
      // Grow snake and increase speed.
      GrowBody();
      speed += 0.02;
    }
  }
}