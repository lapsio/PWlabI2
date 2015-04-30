
#include "./include/session.h"



GameSession::GameSession(PhysicsEngine &physicsEngine, Timer &timer, GameDomain &gameDomain) :
  gameDomain(gameDomain),
  timer(timer),
  physicsEngine(physicsEngine),
  gameMap(*physicsEngine.getMap()),
  keyboardInput(new Chain<char>('\0')),
  mouseInpput(PointXY())
{
  this->gameDomain.bindSession(*this);
}

GameSession::GameSession(GameMap &map, Timer &timer, GameDomain &gameDomain) :
  GameSession(*(new PhysicsEngine(map)), timer, gameDomain)
{

}

GameSession::GameSession(int W, int H, Timer &timer, GameDomain &gameDomain) :
  GameSession(*(new GameMap(W,H)), timer, gameDomain)
{

}

bool GameSession::enterSessionLoop(bool &interruptTrigger){
  this->timer.resume();

  Chain<BaseEvent&>* ev=new Chain<BaseEvent&>(*(new BaseEvent()));

  while(!interruptTrigger&&(!ev->next()||ev->next()->data.type!=BaseEvent::Type::swapSessions)){
    while(!this->timer.shift(this->physicsEngine.timeShift()));
    delete ev;
    ev=this->gameDomain.reload();
    //render
  }

  return false;
}
