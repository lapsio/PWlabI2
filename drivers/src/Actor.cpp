#include "./drivers/include/Actor.h"


<<<<<<< HEAD
Actor::Actor(std::string n)
:Object("Actor",Decal("misc/player.png"),PhysicalBody(PhysicalBody::ObjectType::dynamic,PhysicalBody::CollisionType::solid,PointXY(4,8),1,1))
=======
Actor::Actor(std::string n) :
  Object("Player",
         Decal("misc/player.png"),
         PhysicalBody(
           PhysicalBody::ObjectType::dynamic,
           PhysicalBody::CollisionType::solid,
           PointXY(2.5,2.5),
           1,1))
>>>>>>> d018fc2e11606ffdf2b0cf1d99e592e7e3a53a90
{
    this -> pushType(Actor::typeName);
}

Player::~Player() {}
