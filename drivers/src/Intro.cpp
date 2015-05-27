#include "drivers/include/Intro.h"

Intro::~Intro() {}

BaseEvent& Intro::run(GameSession& session)
{
    int i = 1;

    GameSession* game = new GameSession(*session.getRenderEngine(),512,512);
    game ->getGameMap()->addObject(*(new ObjectMapMeta(*(new Player("Player")))));

    Grass* grass = new Grass("Grass");

    for (int i = 0; i<=7; i++)
        for (int k = 0; k<=7; k++)
            game ->getGameMap()->addObject(*(new ObjectMapMeta(*grass,PointXY(64*i,64*k))));

    GameMap & map = *game->getGameMap();
        while ((i < map.length()) && (map[i].object.isTypeOf(Player::typeName) == false))
        {
            i++;
        }
    if (i < map.length())
        game->getPhysicsEngine()->registerObject(map[i]);

    game -> getGameDomain()->add(*(new PlayerController())); /**DODAJE KLOCEK KTORY POWINIEN STEROWAC GRACZEM**/
    Event<GameSession&>* e = new Event<GameSession&> (*game, BaseEvent::Type::swapSessions);
    return *e;
}
