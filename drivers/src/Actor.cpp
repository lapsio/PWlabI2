#include "./drivers/include/Actor.h"


Actor::Actor(std::string n)
:Object("Player",Decal("misc/player.png"),PhysicalBody(PhysicalBody::ObjectType::dynamic,PhysicalBody::CollisionType::solid,PointXY(4,8),2,5))
{
    this -> name = n;
    this -> pushType(Player::typeName);
}

Player::~Player() {}
