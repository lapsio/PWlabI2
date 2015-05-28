#include "drivers/include/Hud.h"

HealthBar::~HealthBar() {}

HealthBar::HealthBar(std::string n)
:Object(n,Decal("misc/OneHB.png",PointXY(-1.25,-1.25)),
                    PhysicalBody(PhysicalBody::ObjectType::passive,PhysicalBody::CollisionType::ghost,PointXY(1.25,1.25),100,1))
{
    this -> pushType(HealthBar::typeName);
}
