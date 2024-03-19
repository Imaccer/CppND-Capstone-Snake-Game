#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class BaseController {
  public:
    virtual void HandleInput(bool &running, Snake &snake) const = 0;
  protected:
    void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

class DefaultController : public BaseController {
public:
  void HandleInput(bool &running, Snake &snake) const override;
};

class AlternativeController : public BaseController {
public:
  void HandleInput(bool &running, Snake &snake) const override;
};


#endif