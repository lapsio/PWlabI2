#include "drivers/include/Static.h"


Grass::Grass(std::string n)
:Object("Grass",Decal("misc/grass.png"),PhysicalBody(PhysicalBody::ObjectType::passive,PhysicalBody::CollisionType::ghost,PointXY(0,0)))
{
    this -> pushType(Grass::typeName);
}
Grass:: ~Grass() {}

Tree::Tree(std::string n)
:Object(n,Decal("misc/Tree-01.png"),
            PhysicalBody(PhysicalBody::ObjectType::passive,PhysicalBody::CollisionType::solid,PointXY(8,8)))
{
    this -> pushType(Tree::typeName);

    Array<PointXY*>* a = Tree::generateColMesh();
    this->reshape(*a);
    a->map([](PointXY* p){delete p;});
    delete a;
}
Tree:: ~Tree() {}
