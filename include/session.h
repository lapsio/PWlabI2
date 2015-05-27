#ifndef SESSION_H
#define SESSION_H

#include "./include/physx.h"
#include "./include/GameDomain.h"
#include "./gfx.h"


/**
 * @brief The GameSession class base game engine class providing session functionality.
 */
class GameSession
{
private:
  RenderEngine& renderEngine;
  GameDomain& gameDomain;
  Timer& timer;
  PhysicsEngine& physicsEngine;
  GameMap& gameMap;

public:
  Chain <char>* keyboardInput; /**< captured keyboard input */
  PointXY mouseInpput; /**< captured mouse input */

  /**
   * @brief GameSession constructor
   * @param physicsEngine physics engine to be used by session
   * @param timer timer to be used by session
   * @param gameDomain game domain code engine to be used by session
   */
  GameSession(RenderEngine& renderEngine, PhysicsEngine& physicsEngine, Timer& timer=*(new Timer(25,true)), GameDomain& gameDomain=*(new GameDomain()));
  GameSession(RenderEngine& renderEngine, GameMap& map, Timer& timer=*(new Timer(25,true)), GameDomain& gameDomain=*(new GameDomain()));
  GameSession(RenderEngine& renderEngine, int W=64, int H=48, Timer& timer=*(new Timer(25,true)), GameDomain& gameDomain=*(new GameDomain()));
  ~GameSession();

  /**
   * @brief enterSessionLoop starts session
   * @param interruptTrigger reference to interruption trigger. when set to true inside game loop, session will terminate
   * @return whether session was interrupted by interrupt trigger or in usual way (session swap event)
   */
  GameSession* enterSessionLoop(bool& interruptTrigger);

  /**
   * @brief getGameMap
   * @return map used by session
   */
  inline GameMap* getGameMap(){return &this->gameMap;}
  /**
   * @brief getTimer
   * @return timer used by session
   */
  inline Timer* getTimer(){return &this->timer;}
  /**
   * @brief getPhysicsEngine
   * @return physics engine used by session
   */
  inline PhysicsEngine* getPhysicsEngine(){return &this->physicsEngine;}
  /**
   * @brief getGameDomain
   * @return game domain engine used by session
   */
  inline GameDomain* getGameDomain(){return &this->gameDomain;}
  /**
   * @brief getRenderEngine
   * @return render engine used by session
   */
  inline RenderEngine* getRenderEngine(){return &this->renderEngine;}
};

#endif // SESSION_H
