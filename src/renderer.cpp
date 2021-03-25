#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, 2, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  // to check the stat of a file need to #include <sys/stat.h>
  // struct stat info;
  // std::string path = "image/sdllogo.bmp"; // To not I get the same behavior with "shaders\\color.vert"
  // int ret = stat(path.c_str(), &info);
  // std::cout << "Render stat: " << ret << std::endl;
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::UpdateScreen() {
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::ClearScreen() {
  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
}

void Renderer::ClearScreenUpdate() {
  // Clear screen
  ClearScreen();
  UpdateScreen();
}

std::unique_ptr<Food> Renderer::RenderFood(std::unique_ptr<Food>&& food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  block.x = food->GetLocation().x * block.w;
  block.y = food->GetLocation().y * block.h;
  // Render food
  if (food->SetImageSurface(sdl_renderer)) {
    // draw the shape:
    SDL_RenderCopy(sdl_renderer, food->GetImageSurface(), nullptr, &block);
  } 
  else {
    // draw color instead of image:
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  return std::move(food);
}

void Renderer::RenderFoodUpdate(std::promise<std::unique_ptr<Food>>&& prms, std::unique_ptr<Food>&& food) {
  std::unique_ptr<Food> food_back = RenderFood(std::move(food));
  prms.set_value(std::move(food_back));
  UpdateScreen();
}

// void Renderer::RenderFood(Food const &food) {
//   SDL_Rect block;
//   block.w = screen_width / grid_width;
//   block.h = screen_height / grid_height;

//   // Render food
//   SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
//   block.x = food.GetLocation().x * block.w;
//   block.y = food.GetLocation().y * block.h;
//   SDL_RenderFillRect(sdl_renderer, &block);
// }

// void Renderer::RenderFoodUpdate(Food const &food) {
//   RenderFood(food);
//   UpdateScreen();
// }

void Renderer::RenderSnakesBody(Snake const& snake) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }
}

void Renderer::RenderSnakesHead(Snake const& snake) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::ClearLastPoint(SDL_Point const& point) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  block.x = point.x * block.w;
  block.y = point.y * block.h;
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::RenderAllSnakeUpdate(Snake const& snake, SDL_Point const& snakeTail) {
  ClearLastPoint(snakeTail);
  RenderSnakesBody(snake);
  RenderSnakesHead(snake);
  UpdateScreen();
}

// void Renderer::Render(Snake const& snake, Food const &food) {
//   ClearScreen();
//   RenderFood(food);
//   RenderSnakesBody(snake);
//   RenderSnakesHead(snake);
//   UpdateScreen();
// }

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
