#ifndef GAME_H
#define GAME_H

// #include "SDL.h"
// #include "controller.h"
// #include "renderer.h"
// #include "snake.h"
// #include <random>

// class Game {
// public:
//   Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_snakes);
//   void Run(BaseController const &controller, Renderer &renderer,
//            std::size_t target_frame_duration);
//   int GetScore() const;
//   int GetSize() const;

// private:
//   Snake snake;
//   SDL_Point food;

//   std::random_device dev;
//   std::mt19937 engine;
//   std::uniform_int_distribution<int> random_w;
//   std::uniform_int_distribution<int> random_h;

//   int score{0};

//   void PlaceFood();
//   void Update();
// };

// game.h
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <vector>
#include <random>
#include <memory>
#include <mutex>

class Game {
public:
    Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_snakes);
    //void Run(std::unique_ptr<BaseController> controller,std::size_t snakeIndex, Renderer &renderer,  std::size_t target_frame_duration);
    void Run(std::vector<std::unique_ptr<BaseController>> controllers, std::size_t snakeIndex, Renderer &renderer,  std::size_t target_frame_duration);
    void Update(std::size_t snakeIndex);
    void PlaceFood();
    int GetScore(std::size_t snakeIndex) const;
    int GetSize(std::size_t snakeIndex) const;

private:
    Snake& GetSnake(std::size_t snakeIndex);
    std::mutex mutex;

    std::vector<Snake> snakes;
    SDL_Point food;
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
    int score{0};
};

#endif

