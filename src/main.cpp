#include "SDL.h"
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <future>
#include <iostream>

int main() {
  constexpr std::size_t kNumThreads{2}; // Number of snakes
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);

  Game game(kGridWidth, kGridHeight, kNumThreads);

  std::vector<std::unique_ptr<BaseController>> controllers;
  controllers.emplace_back(std::make_unique<DefaultController>(game));
  controllers.emplace_back(std::make_unique<AlternativeController>(game));

  game.Run(std::move(controllers), renderer, kMsPerFrame);

  std::cout << "Both snakes have terminated successfully!\n";

  // Print the score and size of each snake
  std::cout << "Snakes "
            << " - Score: " << game.GetScore()
            << ", Sizes: " << game.GetSize()[0] << " and " << game.GetSize()[1]
            << "\n";
  return 0;
}
