#include "./include/physx.h"



ObjectPhysicsMeta::ObjectPhysicsMeta(const ObjectMapMeta& ref, const PhysicsEngine& engine, VectorXY speed, VectorXY acceleration) :
  ObjectMapMeta(ref),
  engine(&engine),
  speed(speed),
  acceleration(acceleration),
  gridChain(nullptr)
{
  this->pushType(ObjectPhysicsMeta::typeName);
}

ObjectPhysicsMeta::~ObjectPhysicsMeta(){
  delete this->gridChain;
  this->engine=nullptr;
}

void ObjectPhysicsMeta::linkMap(GameMap &map){
  if (this->inUse)
    throw "meta already in use";

  if (this->engine->getMap()!=&map)
    throw "cannot place physics meta on different map";

  this->inUse=true;
  this->map=&map;
  this->mapLink=this->link();
}

void ObjectPhysicsMeta::unlinkMap(){
  this->inUse=false;
  this->map=nullptr;
  delete this->mapLink;
}
