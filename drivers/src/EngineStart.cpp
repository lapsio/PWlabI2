#include "drivers/include/EngineStart.h"

EngineStart::~EngineStart () {}

BaseEvent& EngineStart:: run(GameSession& session)
{
    GameSession* startup = new GameSession(this -> renderEngine);

    startup->getGameMap()->addObject(*(new ObjectMapMeta(*(new Object("Splash",
                                                                            Decal("misc/derp.png"))),
                                                                 PointXY(startup->getGameMap()->width,
                                                                         startup->getGameMap()->height))));

    return *(new BaseEvent);
}
