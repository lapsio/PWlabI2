#ifndef ENGINE_H
#define ENGINE_H

#include "include/session.h"
#include "include/gfx.h"

class GameEngine
{
private:
  RenderEngine& renderEngine;
  Chain<GameSession&>* sessions;

public:
  GameEngine();
  ~GameEngine();

  void start(bool& interruptTrigger);
};

#endif // ENGINE_H
