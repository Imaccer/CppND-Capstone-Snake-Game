#include "game.h"
#include "SDL.h"
#include <iostream>


Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_snakes)
    : engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
    for (std::size_t i = 0; i < num_snakes; ++i) {
        snakes.emplace_back(grid_width, grid_height, i); // Pass the snake ID as well
    }
    PlaceFood();
}

// void Game::Run(BaseController const &controller, Renderer &renderer,
//                std::size_t target_frame_duration) {
//   Uint32 title_timestamp = SDL_GetTicks();
//   Uint32 frame_start;
//   Uint32 frame_end;
//   Uint32 frame_duration;
//   int frame_count = 0;
//   bool running = true;

//   while (running) {
//     frame_start = SDL_GetTicks();

//     // Input, Update, Render - the main game loop.
//     controller.HandleInput(running, snake);
//     Update();
//     renderer.Render(snake, food);

//     frame_end = SDL_GetTicks();

//     // Keep track of how long each loop through the input/update/render cycle
//     // takes.
//     frame_count++;
//     frame_duration = frame_end - frame_start;

//     // After every second, update the window title.
//     if (frame_end - title_timestamp >= 1000) {
//       renderer.UpdateWindowTitle(score, frame_count);
//       frame_count = 0;
//       title_timestamp = frame_end;
//     }

//     // If the time for this frame is too small (i.e. frame_duration is
//     // smaller than the target ms_per_frame), delay the loop to
//     // achieve the correct frame rate.
//     if (frame_duration < target_frame_duration) {
//       SDL_Delay(target_frame_duration - frame_duration);
//     }
//   }
// }

//void Game::Run(std::unique_ptr<BaseController> controller, std::size_t snakeIndex, Renderer &renderer,  std::size_t target_frame_duration) {

void Game::Run(std::vector<std::unique_ptr<BaseController>> controllers, std::size_t snakeIndex, Renderer &renderer,  std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    bool running = true;

    while (running) {
        frame_start = SDL_GetTicks();

        // Poll SDL events
        SDL_Event e;

        std::unique_lock<std::mutex> lock(mutex);
        while (SDL_PollEvent(&e)) {
            // Pass the event to both controllers
            for (int i = 0; i < controllers.size(); i++) {
              controllers[i]->HandleInput(running, snakes[i], e);
            }
        }
           
        // Input, Update, Render - the main game loop.
        // controller->HandleInput(running, snakes[snakeIndex]);
    
        Update(snakeIndex);
        lock.unlock();
        renderer.Render(snakes[snakeIndex], food); // Render the snake and food
        
        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000) {
            //renderer.UpdateWindowTitle(score, frame_count);
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

void Game::Update(std::size_t snakeIndex) {

    Snake& snake = snakes[snakeIndex];

    if (!snake.alive)
        return;

    snake.Update();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    // Check if there's food over here
    if (food.x == new_x && food.y == new_y) {
        score++;
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
    }
}

void Game::PlaceFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        // Check that the location is not occupied by a snake item before placing
        // food.
        bool isFoodPlaced = true;
        for (auto& snake : snakes) {
            if (snake.SnakeCell(x, y)) {
                isFoodPlaced = false;
                break;
            }
        }
        if (isFoodPlaced) {
            food.x = x;
            food.y = y;
            return;
        }
    }
}

int Game::GetScore(std::size_t snakeIndex) const {
    return score;
}

int Game::GetSize(std::size_t snakeIndex) const {
    return snakes[snakeIndex].size;
}

Snake& Game::GetSnake(std::size_t snakeIndex) {
    return snakes[snakeIndex];
}
