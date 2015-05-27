#include "drivers/include/EngineStart.h"

EngineStart::~EngineStart () {}

BaseEvent& EngineStart:: run(GameSession& session)
{

  if (IsObjectAdded == false)
  {
    std::cout << "Adding Splash" << std::endl;
    session.getGameMap()->addObject(*(new ObjectMapMeta(*(new Object("Splash",
                                                                     Decal("misc/derp.png"))),
                                                        PointXY(0,0))));
    this -> setIsObjectAdded(true);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
  {
    std::cout << "Key detected" << std::endl;
    session.getGameDomain()->add(*(new Intro())); /**DODAJE KLOCEK KTORY W RUN ZWRACA swapSESSIONS**/
  }

  return *(new BaseEvent);
}

void EngineStart:: setIsObjectAdded (bool C)
{
  IsObjectAdded = C;
  return;
}
