
#include "./include/session.h"



GameSession::GameSession(RenderEngine &renderEngine, PhysicsEngine &physicsEngine, Timer &timer, GameDomain &gameDomain) :
  renderEngine(renderEngine),
  gameDomain(gameDomain),
  timer(timer),
  physicsEngine(physicsEngine),
  gameMap(*physicsEngine.getMap()),
  keyboardInput(new Chain<char>('\0')),
  mouseInpput(PointXY())
{
  this->gameDomain.bindSession(*this);
}

GameSession::GameSession(RenderEngine &renderEngine, GameMap &map, Timer &timer, GameDomain &gameDomain) :
  GameSession(renderEngine, *(new PhysicsEngine(map)), timer, gameDomain)
{

}

GameSession::GameSession(RenderEngine &renderEngine, int W, int H, Timer &timer, GameDomain &gameDomain) :
  GameSession(renderEngine, *(new GameMap(W,H)), timer, gameDomain)
{

}

GameSession* GameSession::enterSessionLoop(bool &interruptTrigger){
  this->timer.resume();

  Chain<BaseEvent&>* ev=new Chain<BaseEvent&>(*(new BaseEvent()));

  while(!interruptTrigger&&(!ev->next()||ev->next()->data.type!=BaseEvent::Type::swapSessions)){
    while(!this->timer.shift(this->physicsEngine.timeShift()));
    delete ev;
    ev=this->gameDomain.reload();
    this->renderEngine.clear();

    for (int i = 1, l = this->gameMap.length(); i < l ; i++){
      std::cout << this->gameMap[i].object.getName() << std::endl;
      this->renderEngine.pushObject(this->gameMap[i].getGlobalPos(),this->gameMap[i].object);
    }
    this->renderEngine.flush();
  }

  if (interruptTrigger)
    return nullptr;

  return &((dynamic_cast<Event<GameSession&>*> (&(ev->next()->data)))->data);
}
