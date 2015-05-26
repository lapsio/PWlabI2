#include "include/engine.h"

GameEngine::GameEngine() :
  renderEngine(*(new RenderEngine(true))),
  sessions(new Chain<GameSession&>(*(new GameSession(this->renderEngine))))
{
    this->sessions->data.getGameDomain()->add(*(new EngineStart(this->renderEngine)));
}

GameEngine::~GameEngine(){

}

void GameEngine::start(bool &interruptTrigger){
  GameSession* newSession=&this->sessions->data;

  while(1){
    newSession = newSession->enterSessionLoop(interruptTrigger);
    if (newSession==nullptr)
      break;

    Chain<GameSession&>*c=this->sessions;
    while(c){//check if session already registered
      if (&c->data==newSession)
        break;
      c=c->next();
    }
    if (!c)//if not then add new session to list
      this->sessions->rewindForward()->insertAfter(*newSession);
  }

  return;
}
