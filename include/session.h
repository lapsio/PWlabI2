#ifndef SESSION_H
#define SESSION_H

#include "./include/physx.h"
#include "./include/GameDomain.h"

class GameSession
{
private:
  GameDomain& gameDomain;
  Timer& timer;
  PhysicsEngine& physicsEngine;
  GameMap& gameMap;

public:
  Chain <char>* keyboardInput;
  PointXY mouseInpput;

  GameSession(PhysicsEngine& physicsEngine, Timer& timer=*(new Timer(25,true)), GameDomain& gameDomain=*(new GameDomain()));
  GameSession(GameMap& map, Timer& timer=*(new Timer(25,true)), GameDomain& gameDomain=*(new GameDomain()));
  GameSession(int W=128, int H=96, Timer& timer=*(new Timer(25,true)), GameDomain& gameDomain=*(new GameDomain()));
  ~GameSession();

  bool enterSessionLoop(bool& interruptTrigger);

  inline GameMap* getGameMap(){return &this->gameMap;}
  inline Timer* getTimer(){return &this->timer;}
  inline PhysicsEngine* getPhysicsEngine(){return &this->physicsEngine;}
  inline GameDomain* getGameDomain(){return &this->gameDomain;}
};

class GameEngine
{
private:
  Chain<GameSession>* sessions;

public:
  GameEngine();
  ~GameEngine();

  void run(bool& interruptTrigger);
};

#endif // SESSION_H
