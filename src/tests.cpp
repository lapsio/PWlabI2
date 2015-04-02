#include <stdio.h>
#include "./include/tests.h"
#include "./include/physx.h"

void test_punkt_konstruktor()
{
    PointXY A;
    if (A.getX() != 0 || A.getY() != 0)
        cout << "Blad dla 1 konstruktora z domyslymi wartosciami" << endl;

    PointXY B(5 , 5);
    if (B.getX() != 5 || B.getY() != 5)
        cout << "Blad dla 1 konstruktora z podanymi wartosciami" << endl;

    PointXY C(B);
    if (C.getX() != 5 || C.getY() != 5)
        cout << "Blad dla 2 konstruktora z podanym punktem" << endl;

    /** Punkt A(0,0) , B(5,5) , C(5,5) **/
}

void test_punkt_changeTo(long double k , long double l)
{
    PointXY B , A;
    B.changeTo(k , l);
    if (B.getX() != k || B.getY() != l)
        cout << "Blad dla changeTo z podanymi wartosciami" << endl;


    A.changeTo(B);
    if (A.getX() != k || A.getY() != l)
        cout << "Blad dla changeTo z podanym punktem" << endl;


    return;
}
void test_punkt_moveBy(long double k,long double l)
{
    PointXY A;
    PointXY B(-k , -l);

    A.moveBy(k , l);
    if (A.getX() != k || A.getY() != l)
        cout << "Blad dla moveBy z podanymi wartosciamy" << endl;

    /** Punkt A(0,0) , B(-1,-1) , C(5,5) **/

    A.moveBy(B);
    if (A.getX() != 0 || A.getY() != 0)
        cout << "Blad dla moveBy z podanym punktem" << endl;

}

void test_punkt_operatory(long double ax, long double ay, long double bx , long double by)
{
    PointXY C;
    PointXY A(ax,ay);
    PointXY B(bx,by);
    C = A + B;
    if (C.getX() != ax + bx || C.getY() != ay + by)
        cout << "Blad dla operatora +" << endl;



    C = C - 2*B;
    if (C.getX() != ax + bx - 2*bx || C.getY() != ay + by - 2*by)
        cout << "Blad dla operatora - albo k*Punkt" << endl;

    if ((A == B && (ax != bx && ay != by)) || (A != B && (ax == bx && ay == by)))
        cout << "Blad dla operatora == lub !=" << endl;
}
void test_punkt()
{
    test_punkt_konstruktor();

    test_punkt_changeTo(10, 10);

    test_punkt_moveBy(5, 5);

    test_punkt_operatory(2 , 2 , 3 , 3);


    return;
}

void test_wektor_konstruktory()
{
    VectorXY AB;
    VectorXY CD (-1,-1,1,1);
    PointXY Q(-1, -1);
    PointXY W (1 , 1);
    if (AB.getBegin().getX() != 0 || AB.getBegin().getY() != 0 || AB.getEnd().getX() != 1 || AB.getEnd().getY() != 1)
        cout << "Blad dla konstruktora domyslnego" << endl;

    if (CD.getBegin().getX() != -1 || CD.getBegin().getY() != -1 || CD.getEnd().getX() != 1 || CD.getEnd().getY() != 1)
        cout << "Blad dla konstruktora z podanymi wartosciami" << endl;

    VectorXY QW(Q,W);
    if (QW.getBegin().getX() != -1 || QW.getBegin().getY() != -1 || QW.getEnd().getX() != 1 || QW.getEnd().getY() != 1)
        cout << "Blad dla konstruktora z podanymi punktami" << endl;

    VectorXY D(QW);
    if (D.getBegin().getX() != -1 || D.getBegin().getY() != -1 || D.getEnd().getX() != 1 || D.getEnd().getY() != 1)
        cout << "Blad dla konstruktora z podanym wektorem" << endl;

    return;
}

void test_wektor_set()
{
    VectorXY AB;
    PointXY A(1,1);
    PointXY B (2,2);
    VectorXY CD;
    CD.setVector(A,B);
    AB.setBegin(1 , 1);
    AB.setEnd(2,2);

    if (AB.getBegin().getX() != 1 || AB.getBegin().getY() != 1 || AB.getEnd().getX() != 2 || AB.getEnd().getY() != 2)
        cout << "Blad dla setBegin lub setEnd z podanymi wartosciami" << endl;
    if (CD.getBegin().getX() != 1 || CD.getBegin().getY() != 1 || CD.getEnd().getX() != 2 || CD.getEnd().getY() != 2)
        cout << "Blad dla setVector, setBegin, setEnd z podanymi punktami" << endl;

    return;

}

void test_wektor_wymiary(long double k, long double l)
{
    VectorXY AB(0,0,k,l);

    if (AB.height() != k || AB.width() != k)
        cout << "Blad dla height i width" << endl;
    if (AB.size() != sqrt(k*k + l*l))
        cout << "Blad dla size()" << endl;
    PointXY C;
    C = AB.sizeXY();
    if (C.getX() != k && C.getY() != l)
        cout << "Blad sizeXY" << endl;

    return;
}

void test_wektor_move(long double k , long double l)
{
    VectorXY AB;
    AB.moveBy(k , l);
    PointXY A(-k,-l);
    if (AB.getBegin().getX() != 0 + k || AB.getBegin().getY() != 0 + l || AB.getEnd().getX() != 1+k || AB.getEnd().getY() != 1+l)
        cout << "Blad dla moveby z wartosciami" << endl;
    AB.moveBy(A);
    if (AB.getBegin().getX() != 0 || AB.getBegin().getY() != 0 || AB.getEnd().getX() != 1 || AB.getEnd().getY() != 1)
        cout << "Blad dla moveby z punktem" << endl;

    AB.moveTo(k,l);
    if (AB.getBegin().getX() != k || AB.getBegin().getY() != l)
        cout << "Blad dla moveTo dla poczatku" << endl;
    AB.moveTo(k,l,true);
    if (AB.getEnd().getX() != k || AB.getEnd().getY() != l)
        cout << "Blad dla moveTo dla konca" << endl;

    return;
}

void test_wektor_centerOn()
{
    VectorXY AB(-1,-1,1,1);
    AB.centerOn(1,1);
    PointXY C(-1,-1);

    if (AB.getBegin().getX() != 0 || AB.getBegin().getY() != 0 || AB.getEnd().getX() != 2 || AB.getEnd().getY() != 2)
        cout << "Blad dla CenterOn z podanymi wartosciami" << endl;

    AB.centerOn(C);

    if (AB.getBegin().getX() != -2 || AB.getBegin().getY() != -2 || AB.getEnd().getX() != 0 || AB.getEnd().getY() != 0)
        cout << "Blad dla CenterOn z podanymi wartosciami" << endl;

    return;
}

void test_wektor()
{
    test_wektor_konstruktory();
    test_wektor_set();
    test_wektor_wymiary(10,10);
    test_wektor_move(10,10);
    test_wektor_centerOn();
}

void CORE_DEBUG(){

  ////OBJECTS/////////////////////////////////////////////////////////////

  Object * Circle = new Object(
                   "potato",
                   Decal(),
                   PhysicalBody(
                     PhysicalBody::dynamic,
                     PhysicalBody::solid,
                     PointXY(2,5),
                     4,10
                     ));

  Array<PointXY*>arr(5);

  arr[0]=new PointXY(0,0);
  arr[1]=new PointXY(1,0);
  arr[2]=new PointXY(1,0.6);
  arr[3]=new PointXY(0.5,1);
  arr[4]=new PointXY(0,0.6);

  Array<PointXY*>badarr(2);
  badarr[0]=new PointXY(0,0);
  badarr[1]=new PointXY(1,0);


  std::cout << std::endl << std::endl << "try to construct object with 2-vertex collision mesh: " << std::endl;

  try{
    new Object("box",Decal(),
               PhysicalBody(
                 PhysicalBody::dynamic,
                 PhysicalBody::solid,
                 PointXY(4,8),
                 badarr,2,5
                 ));
    std::cout << "failure - function call succesfull" << std::endl;
  } catch (const char*) {
    std::cout << "success - function call failed as expected" << std::endl;
  }

  badarr.map([](PointXY*& v){delete v;});

  Object * Mesh = new Object("box",Decal(),
                             PhysicalBody(
                               PhysicalBody::dynamic,
                               PhysicalBody::solid,
                               PointXY(4,8),
                               arr,2,5
                               ));

  PhysicalBody::DEBUG(Circle);
  Object::DEBUG(Circle);
  PhysicalBody::DEBUG(Mesh);
  Object::DEBUG(Mesh);

  Object * CPY=new Object(*Mesh);

  std::cout << "Circle users:" << Circle->refUsersCount() << std::endl;
  std::cout << "Mesh users:" << Mesh->refUsersCount() << std::endl;
  std::cout << "CPY users:" << Mesh->refUsersCount() << std::endl;

  GCRefLink * cref = Circle->link();
  GCRefLink * mref = Mesh->link();

  std::cout << "Circle users:" << Circle->refUsersCount() << std::endl;
  std::cout << "Mesh users:" << Mesh->refUsersCount() << std::endl;
  std::cout << "CPY users:" << CPY->refUsersCount() << std::endl;
  std::cout << "Circle meshptr:" << &Circle->getCollisionMesh() << std::endl;
  std::cout << "Mesh meshptr:" << &Mesh->getCollisionMesh() << std::endl;
  std::cout << "CPY meshptr:" << &CPY->getCollisionMesh() << std::endl;

  delete cref;
  delete mref;

  std::cout << "Circle users:" << Circle->refUsersCount() << std::endl;
  std::cout << "Mesh users:" << Mesh->refUsersCount() << std::endl;
  std::cout << "Circle meshptr:" << &Circle->getCollisionMesh() << std::endl;
  std::cout << "Mesh meshptr:" << &Mesh->getCollisionMesh() << std::endl;

  ///META//////////////////////////////////////////////////////////////////////////

  Circle = new Object(
                   "potato",
                   Decal(),
                   PhysicalBody(
                     PhysicalBody::dynamic,
                     PhysicalBody::solid,
                     PointXY(2,5),
                     4,10
                     ));

  Mesh = new Object("box",Decal(),
                             PhysicalBody(
                               PhysicalBody::dynamic,
                               PhysicalBody::solid,
                               PointXY(4,8),
                               arr,2,5
                               ));

  ObjectMapMeta * Cmeta = new ObjectMapMeta(*Circle,PointXY(1,5));
  ObjectMapMeta * Mmeta = new ObjectMapMeta(*Mesh);

  ObjectMapMeta * CPYmeta = new ObjectMapMeta(*Mmeta);

  std::cout << std::endl << std::endl << "fresh: " << std::endl;

  ObjectMapMeta::DEBUG(Cmeta);
  ObjectMapMeta::DEBUG(Mmeta);
  ObjectMapMeta::DEBUG(CPYmeta);

  std::cout << std::endl << std::endl << "attached: " << std::endl;

  Cmeta->attach(*CPYmeta);

  ObjectMapMeta::DEBUG(Cmeta);
  ObjectMapMeta::DEBUG(CPYmeta);

  CPYmeta->unlock();

  std::cout << std::endl << std::endl << "after delete: " << std::endl;

  ObjectMapMeta::DEBUG(Cmeta);

  std::cout << std::endl << std::endl << "mesh data: " << std::endl;

  Object::DEBUG(Mesh);
  Mmeta->unlock();

  std::cout << std::endl << std::endl << "mesh pointer after meta delete: " << std::endl;

  std::cout << "pointer: " << &Mesh->getCollisionMesh() << std::endl;
  Cmeta->unlock();

  ///MAP//////////////////////////////////////////////////////////////////////////

  Circle = new Object(
                   "potato",
                   Decal(),
                   PhysicalBody(
                     PhysicalBody::dynamic,
                     PhysicalBody::solid,
                     PointXY(2,5),
                     4,10
                     ));

  Mesh = new Object("box",Decal(),
                             PhysicalBody(
                               PhysicalBody::dynamic,
                               PhysicalBody::solid,
                               PointXY(4,8),
                               arr,2,5
                               ));

  Mmeta = new ObjectMapMeta(*Mesh);
  CPYmeta = new ObjectMapMeta(*Mmeta);

  GameMap* map = new GameMap();

  map->addObject(*Circle,PointXY(1,2));
  map->addObject(*Mmeta);
  map->addObject(*CPYmeta);
  Cmeta = &map->getMeta(1);

  std::cout << std::endl << std::endl << "fresh map: " << std::endl;

  GameMap::DEBUG(map);

  Cmeta->attach(*Mmeta);

  std::cout << std::endl << std::endl << "after attach: " << std::endl;

  GameMap::DEBUG(map);
  map->deleteObject(Mmeta);

  std::cout << std::endl << std::endl << "after delete: " << std::endl;

  GameMap::DEBUG(map);

  std::cout << std::endl << std::endl << "mesh pointer after first meta delete: " << std::endl;

  std::cout << "pointer: " << &Mesh->getCollisionMesh() << std::endl;

  std::cout << std::endl << std::endl << "try to pop last meta without destroying: " << std::endl;

  try{
    map->popObject(CPYmeta);
    std::cout << "failure - function call succesfull" << std::endl;
  } catch (const char*) {
    std::cout << "success - function call failed as expected" << std::endl;
  }

  map->deleteObject(CPYmeta);

  std::cout << std::endl << std::endl << "mesh pointer after all meta delete: " << std::endl;

  std::cout << "pointer: " << &Mesh->getCollisionMesh() << std::endl;

  std::cout << std::endl << std::endl << "try to delete unexisting entry: " << std::endl;

  try{
    map->deleteObject(CPYmeta);
    std::cout << "failure - function call succesfull" << std::endl;
  } catch (const char* msg){
    std::cout << "success - function call failed as expected" << std::endl;
  }

  std::cout << "Hello World! omg" << std::endl;
}