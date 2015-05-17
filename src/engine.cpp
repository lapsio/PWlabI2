
#include "include/engine.h"

GameEngine::GameEngine() :
  renderEngine(*(new RenderEngine(true))),
  sessions(new Chain<GameSession&>(*(new GameSession(this->renderEngine))))
{
  this->sessions->insertAfter(*(new GameSession(this->renderEngine)));
}

GameEngine::~GameEngine(){

}

void GameEngine::start(bool &interruptTrigger){
  this->sessions->data.enterSessionLoop(interruptTrigger);
}
