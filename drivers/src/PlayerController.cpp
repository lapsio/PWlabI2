#include "drivers/include/PlayerController.h"

PlayerController:: ~PlayerController() {}

void PlayerController:: setPlayerHolder (ObjectMapMeta* MM)
{
    PlayerHolder = dynamic_cast<ObjectPhysicsMeta*> (MM);
}


BaseEvent& PlayerController:: run(GameSession& session)
{
    int i = 1;

    if (PlayerHolder == nullptr)
    {
        GameMap & map = *session.getGameMap();
        while (i < map.length() && (map[i].object.isTypeOf(Player::typeName) == false))
        {
            i++;
        }
        if (i<map.length())
        {
            this -> setPlayerHolder(&map[i]);
        }

    }
    if (PlayerHolder != nullptr)
    {

        VectorXY W (0,0,0,0);
        VectorXY S (0,0,0,0);
        VectorXY A (0,0,0,0);
        VectorXY D (0,0,0,0);
        VectorXY WASD (0,0,0,0);


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            W.setEnd(0,-0.02);
            W.show();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            S.setEnd(0,0.02);
            S.show();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            A.setEnd(-0.02,0);
            A.show();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            D.setEnd(0.02,0);
            D.show();
        }

        WASD = W + S + A + D;
        WASD.show();
        //std::cout << WASD.height() << " " << WASD.width() << std::endl;


        PlayerHolder->speed.setEnd(WASD.getEnd());
        PlayerHolder->speed.setBegin(WASD.getBegin());
    }

    return *(new BaseEvent);

}

