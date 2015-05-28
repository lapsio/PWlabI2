#include "drivers/include/Intro.h"
#include "drivers/include/Hud.h"

Intro::~Intro() {}

BaseEvent& Intro::run(GameSession& session)
{
    int i = 1;

    GameSession* game = new GameSession(*session.getRenderEngine(),64,64);

    Grass* grass = new Grass("Grass");
    Tree* Tree01 = new Tree("Tree");

    ObjectMapMeta* player = new ObjectMapMeta(*(new Player("Player",1,1,1)));
    ObjectMapMeta* HB = new ObjectMapMeta(*(new HealthBar("HealthBar")),PointXY(-32,26));

    for(int i = 0; i < 10; i++)
        for (int k = 0; k < 10; k++)
            game ->getGameMap()->addObject(*(new ObjectMapMeta(*grass,PointXY(6.4*i, 6.4*k))));

    game ->getGameMap()->addObject(*(player));
    game ->getGameMap()->addObject(*(HB));

    game->getPhysicsEngine()->registerObject(*player);

    GameMap & map = *game->getGameMap();
    i = 1;
    while (i < map.length() && (map[i].object.isTypeOf(Player::typeName) == false))
        {
            i++;
        }
    if (i < map.length())
    {
        HB -> attach(map[i]);
    }
    srand( time( NULL ) );
    int liczba1;
    int liczba2;

    for(int i = 0; i < 15; i++)
    {
        liczba1 =( std::rand() % 48 ) + 8;
        liczba2 =( std::rand() % 48 ) + 8;
        game ->getGameMap()->addObject(*(new ObjectMapMeta(*Tree01,PointXY(liczba1, liczba2))));
        game -> getPhysicsEngine()->registerObject(map[1]);
    }

    game -> getGameDomain()->add(*(new PlayerController())); /**DODAJE KLOCEK KTORY POWINIEN STEROWAC GRACZEM**/
    Event<GameSession&>* e = new Event<GameSession&> (*game, BaseEvent::Type::swapSessions);
    return *e;
}
