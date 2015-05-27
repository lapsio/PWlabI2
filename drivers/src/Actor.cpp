#include "./drivers/include/Actor.h"


Actor::Actor(std::string n)
:Object("Player",Decal("misc/player.png"),PhysicalBody(PhysicalBody::ObjectType::dynamic,PhysicalBody::CollisionType::solid,PointXY(4,8),1,1))
{
    this -> name = n;
    this -> pushType(Actor::typeName);
}

Player::~Player() {}
