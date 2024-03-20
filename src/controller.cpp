#include "controller.h"
#include "SDL.h"
#include "game.h"
#include "snake.h"
#include <iostream>

void BaseController::ChangeDirection(Snake &snake, Snake::Direction input,
                                     Snake::Direction opposite) const {
  std::lock_guard<std::mutex> lock(mutex_);
  if (snake.direction != opposite || snake.size == 1)
    snake.direction = input;
  return;
}

void DefaultController::HandleInput(bool &running, Snake &snake) const {
  while (running) {
    SDL_Event e;
    {
      std::unique_lock<std::mutex> lock(game.eventMutex);
      game.eventCV.wait(lock, [&] { return !game.eventQueue.empty(); });
      e = game.eventQueue.front();
    }

    if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
            {
        // Notify that this controller has finished processing the event
        std::lock_guard<std::mutex> lock(game.processedMutex);
        game.processedCount++;
        game.processedCV.notify_one();
      }
    }

    // Handle SDL event
    if (e.type == SDL_QUIT) {
      running = false;

    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
        break;
      case SDLK_DOWN:
        ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
        break;
      case SDLK_LEFT:
        ChangeDirection(snake, Snake::Direction::kLeft,
                        Snake::Direction::kRight);
        break;
      case SDLK_RIGHT:
        ChangeDirection(snake, Snake::Direction::kRight,
                        Snake::Direction::kLeft);
        break;
      }

    }
  }
}

void AlternativeController::HandleInput(bool &running, Snake &snake) const {
  while (running) {
    SDL_Event e;
    {
      std::unique_lock<std::mutex> lock(game.eventMutex);
      game.eventCV.wait(lock, [&] { return !game.eventQueue.empty(); });
      e = game.eventQueue.front();
    }

    if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
            {
        // Notify that this controller has finished processing the event
        std::lock_guard<std::mutex> lock(game.processedMutex);
        game.processedCount++;
        game.processedCV.notify_one();
      }
    }


    // Handle SDL event
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_w:
        ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
        break;
      case SDLK_s:
        ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
        break;
      case SDLK_a:
        ChangeDirection(snake, Snake::Direction::kLeft,
                        Snake::Direction::kRight);
        break;
      case SDLK_d:
        ChangeDirection(snake, Snake::Direction::kRight,
                        Snake::Direction::kLeft);
        break;
      }


    }
  }
}