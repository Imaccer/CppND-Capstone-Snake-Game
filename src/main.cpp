#include "game.h"
#include "SDL.h"
#include <iostream>
#include <future>
#include "controller.h"
#include "renderer.h"

int main() {
    constexpr std::size_t kNumThreads{2}; // Number of snakes
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    
    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);

    std::vector<std::unique_ptr<BaseController>> controllers;
    controllers.emplace_back(std::make_unique<DefaultController>());
    controllers.emplace_back(std::make_unique<AlternativeController>());

    Game game(kGridWidth, kGridHeight, kNumThreads);

    // std::vector<std::future<void>> futures;
    game.Run(std::move(controllers), renderer, kMsPerFrame);

    // for (std::size_t i = 0; i < kNumThreads; ++i) {
    //     futures.emplace_back(std::async(std::launch::async, [&](std::size_t snakeIndex) {
    //           game.Run(std::move(controllers), snakeIndex, std::ref(renderer), kMsPerFrame);

    //     }, i));
    // }

    // for (auto& future : futures) {
    //     future.wait();
    // }

    std::cout << "Both snakes have terminated successfully!\n";

    // Print the score and size of each snake
    // for (std::size_t i = 0; i < kNumThreads; ++i) {
        std::cout << "Snakes " << " - Score: " << game.GetScore() << ", Sizes: " << game.GetSize()[0] << " and " << game.GetSize()[1] << "\n";
    // }

    return 0;
}
