#include "drivers/include/Intro.h"

Intro::~Intro() {}

BaseEvent& Intro::run(GameSession& session)
{
    int i = 1;

    GameSession* game = new GameSession(*session.getRenderEngine(),64,64);

    Grass* grass = new Grass("Grass");
    Tree* Tree01 = new Tree("Tree");

    for(int i = 0; i < 10; i++)
        for (int k = 0; k < 10; k++)
            game ->getGameMap()->addObject(*(new ObjectMapMeta(*grass,PointXY(6.4*i, 6.4*k))));

    game ->getGameMap()->addObject(*(new ObjectMapMeta(*(new Player("Player",1,1,1)))));

    GameMap & map = *game->getGameMap();
        while (i < map.length() && (map[i].object.isTypeOf(Player::typeName) == false))
        {
            i++;
        }
    if (i < map.length())
        game->getPhysicsEngine()->registerObject(map[i]);

    game ->getGameMap()->addObject(*(new ObjectMapMeta(*Tree01,PointXY(16, 16))));
    i = 1;
        while (i < map.length() && (map[i].object.isTypeOf(Tree::typeName) == false))
        {
            i++;
        }
    if (i < map.length())
        game->getPhysicsEngine()->registerObject(map[i]);


    game -> getGameDomain()->add(*(new PlayerController())); /**DODAJE KLOCEK KTORY POWINIEN STEROWAC GRACZEM**/
    Event<GameSession&>* e = new Event<GameSession&> (*game, BaseEvent::Type::swapSessions);
    return *e;
}
