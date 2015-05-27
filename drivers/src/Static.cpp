#include "drivers/include/Static.h"


Grass::Grass(std::string n)
:Object("Grass",Decal("misc/grass.png"),PhysicalBody(PhysicalBody::ObjectType::passive,PhysicalBody::CollisionType::ghost,PointXY(0,0)))
{
    this -> pushType(Grass::typeName);
}
Grass:: ~Grass() {}
