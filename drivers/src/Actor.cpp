#include "./drivers/include/Actor.h"


Actor::Actor(std::string n, float maxhp, float curhp, int lvl)
:Object(n,Decal("misc/player.png"),
                    PhysicalBody(PhysicalBody::ObjectType::dynamic,PhysicalBody::CollisionType::solid,PointXY(2.5,2.5),1,1))
{
    this -> pushType(Actor::typeName);

    this->CurrentHealthPoint = curhp;
    this->MaxHealthPoint = maxhp;
    this->Level = lvl;
}

Player::~Player() {}

Player::Player(std::string N, float maxhp, float curhp, int lvl)
:Actor(N , maxhp, curhp, lvl)
{
    this->pushType(Player::typeName);
}

void Actor::setMaxHealthPoint (float mhp)
{
    this->MaxHealthPoint = mhp;
}
void Actor::setCurrentHealthPoint (float curhp)
{
    this->CurrentHealthPoint = curhp;
}
void Actor::setLevel (int lvl)
{
    this->Level = lvl;
}
