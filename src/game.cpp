#include "game.h"
#include "SDL.h"
#include <iostream>
#include <thread>


Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t num_snakes)
    : engine(dev()),
      test("hello"),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
    for (std::size_t i = 0; i < num_snakes; ++i) {
        snakes.emplace_back(grid_width, grid_height, i); // Pass the snake ID as well
    }
    PlaceFood();
}

// Game::~Game() {
//     // Set running flag to false to stop all threads
//     running = false;

//     // Notify all condition variables to wake up any waiting threads
//     eventCV.notify_all(); // Assuming eventCV is a condition variable used in PollEvents()

//     // Join all threads to ensure they finish execution before destroying the game object
//     // if (eventThread.joinable()) {
//     //     eventThread.join();
//     // }
//     // Join other threads if you have any

//     // Clean up SDL resources
//     SDL_Quit(); // Assuming you're using SDL for rendering

//     // Optionally, perform any additional cleanup if needed
// }


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


 // Function to poll SDL events and push them onto the shared queue
// void Game::PollEvents() {
//     SDL_Event e;
//     while (true) {
//         if (SDL_PollEvent(&e)) {
//             std::lock_guard<std::mutex> lock(eventMutex);
//             eventQueue.push(e);
//             eventCV.notify_all();
//         }
//         // Add a delay here if needed to prevent high CPU usage
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
// }   

// Function to poll SDL events and push them onto the shared queue
// void Game::PollEvents() {
//     SDL_Event e;
//     while (running) {
//          if (e.type == SDL_QUIT) {
//             running = false;
//          }
//         if (SDL_PollEvent(&e) && e.type == SDL_KEYDOWN) {
//             std::lock_guard<std::mutex> lock(eventMutex);
//             eventQueue.emplace_back(e);
//             eventCV.notify_all();
//         }
//         // Add a delay here if needed to prevent high CPU usage
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
//     return;
// }


void Game::PollEvents() {
    SDL_Event e;
    while (running) {
        // while (SDL_PollEvent(&e)) {

        if (SDL_PollEvent(&e)) {
            // std::cout << "type" << e.type << "\n";

            if (e.type == SDL_QUIT) {
                // Only push key down events to the event queue
                std::lock_guard<std::mutex> lock(eventMutex);
                eventQueue.emplace_back(e);
                eventCV.notify_all();
                // continue;
                // return;
                // return;
                // running = false; // Set the running flag to false to exit the loop
                // Clear the event queue
                // eventQueue.clear();
            } else if (e.type == SDL_KEYDOWN) {
                // Only push key down events to the event queue
                std::lock_guard<std::mutex> lock(eventMutex);
                eventQueue.emplace_back(e);
                eventCV.notify_all();
                SDL_FlushEvent(SDL_KEYDOWN);
                continue;
            }
        }
        // Add a delay here if needed to prevent high CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // return;
}

void Game::Run(std::vector<std::unique_ptr<BaseController>> controllers, Renderer &renderer,  std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    

    
    std::vector<std::thread> inputThreads;

    
    // Launch threads for each player's input handling
    inputThreads.emplace_back(std::thread(&BaseController::HandleInput, controllers[0].get(), std::ref(running), std::ref(snakes[0])));
    inputThreads.emplace_back(std::thread(&BaseController::HandleInput, controllers[1].get(), std::ref(running), std::ref(snakes[1])));

    std::thread eventThread(&Game::PollEvents, this);

    while (running) {

         frame_start = SDL_GetTicks();

        // Clear the event queue at the beginning of each frame
        // SDL_FlushEvent(SDL_KEYDOWN); // Clear keydown events
        // eventQueue.clear();

        // processedCount = 0;
    

        


    
        Update();

        renderer.Render(snakes, food);
        
        eventQueue.clear();


        // inputThreads.clear();

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

        if (!eventQueue.empty()) {
        // Wait until both controllers have processed the event before popping it from the queue
            std::unique_lock<std::mutex> lock(processedMutex);
            processedCV.wait(lock, [&] { return processedCount >= controllers.size(); });
            eventQueue.pop_front();
            processedCount = 0; // Reset processed count for the next event
        }
        
    }
       eventThread.join();
        for (auto& thread : inputThreads) {
            thread.join();
        }
    
    
}

void Game::Update() {
    for (int i = 0; i < snakes.size(); i++) {
        Snake& snake = snakes[i];

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

int Game::GetScore() const {

    return score;
}

std::vector<int> Game::GetSize() const {
    std::vector<int> size;
    for (int i = 0; i < snakes.size(); i++) {
        size.emplace_back(snakes[i].size);
    }
    return size;
}

// Snake& Game::GetSnake(std::size_t snakeIndex) {
//     return snakes[snakeIndex];
// }
