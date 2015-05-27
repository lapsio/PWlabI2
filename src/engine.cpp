#include "include/engine.h"

GameEngine::GameEngine() :
  renderEngine(*(new RenderEngine(true))),
  sessions(new Chain<GameSession&>(*(new GameSession(this->renderEngine))))
{
    this->sessions->data.getGameDomain()->add(*(new EngineStart(this->renderEngine)));
}

GameEngine::~GameEngine(){
  Chain<GameSession&>*c;
  while((c=this->sessions->next())){
    delete &c->data;
    delete c;
  }
  delete &this->sessions->data;
  delete this->sessions;
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

void GameEngine::purgeSession(GameSession *session){
  Chain<GameSession&>*c=this->sessions;
  while(c){
    if (&c->data==session)
      break;
    c=c->next();
  }
  if (!c)
    throw "Session not found";
  delete &c->data;
  delete c;
}
