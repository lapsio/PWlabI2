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
        while ((map[i].object.isTypeOf(Player::typeName) == false) && i < map.length())
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(-1,0);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(1,0);
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
            {
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(0,1);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(-1,1);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(1,1);
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
            {
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(0,-1);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(-1,-1);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
              std::cout<<"DOWN"<<std::endl;
                PlayerHolder->speed.setBegin(0,0);
                PlayerHolder->speed.setEnd(1,-1);
            }
        }
    }

    return *(new BaseEvent);

}

