#ifndef ENGINE_H
#define ENGINE_H

#include "include/session.h"
#include "include/gfx.h"
#include "drivers/include/EngineStart.h"

/**
 * @brief The GameEngine class main game engine class
 */

class GameEngine
{
private:
  RenderEngine& renderEngine;
  Chain<GameSession&>* sessions;

public:
  GameEngine();
  ~GameEngine();

  /**
   * @brief start start new game
   * @param interruptTrigger reference to bool triggering extraordinary execution interruption
   */
  void start(bool& interruptTrigger);
  /**
   * @brief purgeSession delete specified session (sned it to the mooooon)
   * @param session session to be deleted
   */
  void purgeSession(GameSession* session);
};

#endif // ENGINE_H
