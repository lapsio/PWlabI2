#include "drivers/include/EngineStart.h"

EngineStart::~EngineStart () {}

BaseEvent& EngineStart:: run(GameSession& session)
{

    if (IsObjectAdded == false)
    {
        session.getGameMap()->addObject(*(new ObjectMapMeta(*(new Object("Splash",
                                                                            Decal("misc/derp.png"))),
                                                                                PointXY(session.getGameMap()->width,
                                                                                session.getGameMap()->height))));
        this -> setIsObjectAdded(true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        session.getGameDomain()->add(*(new Intro(this->renderEngine))); /**DODAJE KLOCEK KTORY W RUN ZWRACA swapSESSIONS**/
    }

    return *(new BaseEvent);
}

void EngineStart:: setIsObjectAdded (bool C)
{
    IsObjectAdded = C;
    return;
}
