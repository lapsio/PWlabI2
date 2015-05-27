#include "drivers/include/Intro.h"

Intro::~Intro() {}

BaseEvent& Intro::run(GameSession& session)
{
    int i = 1;

    GameSession* game = new GameSession(*session.getRenderEngine());

    Grass* grass = new Grass("Grass");

    ObjectMapMeta* A = new ObjectMapMeta(*grass,PointXY(0,0));
    ObjectMapMeta* B = new ObjectMapMeta(*grass,PointXY(0,20));

    /*for(int i = 0; i < 8; i++)
        for (int k = 0; k < 8; k++)
            game ->getGameMap()->addObject(*(new ObjectMapMeta(*grass,PointXY(64*i, 64*k))));*/

    game ->getGameMap()->addObject(*(new ObjectMapMeta(*(new Player("Player",1,1,1)))));
    game->getGameMap()->addObject(*A);
    game->getGameMap()->addObject(*B);


    GameMap & map = *game->getGameMap();
        while (i < map.length() && (map[i].object.isTypeOf(Player::typeName) == false))
        {
            i++;
        }
    if (i < map.length())
        game->getPhysicsEngine()->registerObject(map[i]);

    game -> getGameDomain()->add(*(new PlayerController())); /**DODAJE KLOCEK KTORY POWINIEN STEROWAC GRACZEM**/
    Event<GameSession&>* e = new Event<GameSession&> (*game, BaseEvent::Type::swapSessions);
    return *e;
}
