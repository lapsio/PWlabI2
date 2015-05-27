#include <iostream>
#include <string>

#include "./include/object.h"

class Actor: public Object
{
    private:
    public:
        static constexpr char* typeName=(char*)"Actor";

        Actor(std::string n = "Actor");
        virtual ~Actor(){}
};


class Player: public Actor
{
    private:
    public:
        static constexpr char* typeName=(char*)"Player";

        Player(std::string N = "Player")
        :Actor(N) {this->pushType(Player::typeName);}

        virtual ~Player();
};
