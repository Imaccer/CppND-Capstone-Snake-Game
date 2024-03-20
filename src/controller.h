#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
// #include "game.h"
#include <mutex>

class Game;

class BaseController {
  public:
    BaseController(Game& g) : game(g) {};
    virtual void HandleInput(bool &running, Snake &snake) const = 0;
  protected:
    void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
    mutable std::mutex mutex_;
    Game& game;
};

class DefaultController : public BaseController {
public:
    using BaseController::BaseController; // Inherit constructor from BaseController
  void HandleInput(bool &running, Snake &snake) const override;
};

class AlternativeController : public BaseController {
public:
  using BaseController::BaseController; // Inherit constructor from BaseController
  void HandleInput(bool &running, Snake &snake) const override;
};

#endif

