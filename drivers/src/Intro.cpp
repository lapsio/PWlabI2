#include "drivers/include/Intro.h"

Intro::~Intro() {}

BaseEvent& Intro::run(GameSession& session)
{
    GameSession* game = new GameSession(this->renderEngine);
    game ->getGameMap()->addObject(*(new ObjectMapMeta(*(new Player("Player")))));

    game -> getGameDomain()->add(*(new PlayerController())); /**DODAJE KLOCEK KTORY POWINIEN STEROWAC GRACZEM**/
    Event<GameSession&>* e = new Event<GameSession&> (*game, BaseEvent::Type::swapSessions);
    return *e;
}
