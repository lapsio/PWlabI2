#include "./include/object.h"

PhysicalBody::PhysicalBody(ObjectType type,CollisionType collision, PointXY dimensions,double friction,double mass){
  this->setMass(mass);
  this->setFriction(friction);

  this->setObjectType(type);
  this->setCollisionType(collision);
  this->meshType=PhysicalBody::circle;

  this->collisionMesh=this->collisionNormals=nullptr;
}

PhysicalBody::PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, Array<PointXY*> &collisionMesh, double friction, double mass){
  this->setMass(mass);
  this->setFriction(friction);

  this->setObjectType(type);
  this->setCollisionType(collision);
  this->meshType=PhysicalBody::mesh;

  this->collisionMesh=new Array<VectorXY*> (collisionMesh.length());
}

PhysicalBody::~PhysicalBody(){

}

Array<VectorXY*> PhysicalBody::getCollisionMesh(){

}


int Object::maxId=0;

Object::Object(const std::string name,
               const Decal& decal,
               const PhysicalBody& physicalBody,
               const LightSource& lightSource) :
  LightSource(lightSource),
  PhysicalBody(physicalBody)
{
}
