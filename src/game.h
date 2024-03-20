#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <random>
#include <vector>

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_snakes);
  void Run(std::vector<std::unique_ptr<BaseController>> controllers,
           Renderer &renderer, std::size_t target_frame_duration);
  void Update();
  void PlaceFood();
  int GetScore() const;
  std::vector<int> GetSize() const;
  std::deque<SDL_Event> eventQueue;
  std::mutex eventMutex;
  std::condition_variable eventCV;
  bool running = true;
  mutable std::mutex processedMutex;
  std::condition_variable processedCV;
  std::size_t processedCount = 0;

private:
  std::mutex mutex;

  std::vector<Snake> snakes;
  SDL_Point food;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  int score{0};

  void PollEvents();
};

#endif
