#include "controller.h"
#include "SDL.h"
#include "snake.h"
#include <iostream>
#include "game.h"

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
            if (e.type != SDL_QUIT) {
            game.eventQueue.pop_front();
            }
        }

        // Handle SDL event
        std::cout << "default type" << e.type << "\n";
        if (e.type == SDL_QUIT) {
            std::cout << "setting running to false" << "\n";
            running = false;
            // return;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
                // game.eventQueue.pop_front();
                break;
                // return;
            case SDLK_DOWN:
                ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
                break;
                // return;
            case SDLK_LEFT:
                ChangeDirection(snake, Snake::Direction::kLeft, Snake::Direction::kRight);
                break;
                // return;
            case SDLK_RIGHT:
                ChangeDirection(snake, Snake::Direction::kRight, Snake::Direction::kLeft);
                break;
                // return;
                // Handle other key events as needed
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
            if (e.type != SDL_QUIT) {
            game.eventQueue.pop_front();
            }
        }

        // Handle SDL event
        std::cout << "alt type" << e.type << "\n";
        // Handle SDL event
        if (e.type == SDL_QUIT) {
            std::cout << "seting running to false alt" << "\n";
            running = false;
            // return;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_w:
                ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
                break;
            case SDLK_s:
                ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
                break;
                // return;
            case SDLK_a:
                ChangeDirection(snake, Snake::Direction::kLeft, Snake::Direction::kRight);
                break;
                // return;
            case SDLK_d:
                ChangeDirection(snake, Snake::Direction::kRight, Snake::Direction::kLeft);
                break;
            }
        }
    }
}




// // Poll SDL events
//     SDL_Event e;
// while (SDL_PollEvent(&e)) {
//     std::cout << "alternative: "<< e.key.keysym.sym << '\n';
//     if (e.type == SDL_QUIT) {
//         running = false;
//     } else if (e.type == SDL_KEYDOWN) {
//         switch (e.key.keysym.sym) {
//             case SDLK_w:
//                 ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
//                 break;
//             case SDLK_s:
//                 ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
//                 break;
//             case SDLK_a:
//                 ChangeDirection(snake, Snake::Direction::kLeft, Snake::Direction::kRight);
//                 break;
//             case SDLK_d:
//                 ChangeDirection(snake, Snake::Direction::kRight, Snake::Direction::kLeft);
//                 break;
//         }
//     }
// }
// }