
#include "include/engine.h"

GameEngine::GameEngine() :
  renderEngine(*(new RenderEngine(true))),
  sessions(new Chain<GameSession&>(*(new GameSession(this->renderEngine))))
{
  this->sessions->data.getGameMap()->addObject(*(new ObjectMapMeta(*(new Object("Splash",
                                                                            Decal("misc/derp.png"))),
                                                                 PointXY(this->sessions->data.getGameMap()->width,
                                                                         this->sessions->data.getGameMap()->height))));

  this->sessions->insertAfter(*(new GameSession(this->renderEngine)));
}

GameEngine::~GameEngine(){

}

void GameEngine::start(bool &interruptTrigger){
  this->sessions->data.enterSessionLoop(interruptTrigger);
}
