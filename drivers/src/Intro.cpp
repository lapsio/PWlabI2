#include "drivers/include/Intro.h"

Intro::~Intro() {}

BaseEvent& Intro::run(GameSession& session)
{
    int i = 1;

    GameSession* game = new GameSession(*session.getRenderEngine());
    game ->getGameMap()->addObject(*(new ObjectMapMeta(*(new Player("Player")))));

    GameMap & map = *game->getGameMap();
        while ((map[i].object.isTypeOf(Player::typeName) == false) && i < map.length())
        {
            i++;
        }
    if (i < map.length())
        game->getPhysicsEngine()->registerObject(map[i]);

    game -> getGameDomain()->add(*(new PlayerController())); /**DODAJE KLOCEK KTORY POWINIEN STEROWAC GRACZEM**/
    Event<GameSession&>* e = new Event<GameSession&> (*game, BaseEvent::Type::swapSessions);
    return *e;
}
