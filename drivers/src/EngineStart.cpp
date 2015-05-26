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
        return *(new Event <GameSession&> (session, BaseEvent::Type::swapSessions, false));
    }
    else
        return *(new BaseEvent);
}

void EngineStart:: setIsObjectAdded (bool C)
{
    IsObjectAdded = C;
    return;
}
