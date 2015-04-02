#include <stdio.h>
#include "./include/tests.h"
#include "./include/physx.h"

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
