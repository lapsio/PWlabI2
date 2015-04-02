#include "./include/object.h"
#include "./include/gc.h"



PhysicalBody::PhysicalBody(ObjectType type,CollisionType collision, PointXY dimensions,double friction,double mass){
  if (dimensions.getX()<0||dimensions.getY()<0)
    throw "dimensions cannot be negative";

  this->boundBox=dimensions;
  this->setMass(mass);
  this->setFriction(friction);

  this->setObjectType(type);
  this->setCollisionType(collision);

  this->meshType=PhysicalBody::circle;
  this->collisionMesh=this->collisionNormals=nullptr;
  this->reshape();
}

PhysicalBody::PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, Array<PointXY*> &mesh, double friction, double mass) :
  PhysicalBody(type,collision,dimensions,friction,mass)
{
  this->reshape(mesh);
}

PhysicalBody::PhysicalBody(const PhysicalBody &ref) :
  PhysicalBody(ref.type,ref.collisionType,ref.boundBox,ref.friction,ref.mass)
{
  if (ref.meshType==PhysicalBody::mesh){
    this->meshRealloc(*ref.collisionMesh,*ref.collisionNormals);
    this->meshType=PhysicalBody::mesh;
  }
}

PhysicalBody::~PhysicalBody(){
  if (this->meshType==PhysicalBody::mesh){
    this->collisionMesh->map([](VectorXY*& v){delete v;v=nullptr;});
    delete this->collisionMesh;
    this->collisionNormals->map([](VectorXY*& v){delete v;v=nullptr;});
    delete this->collisionNormals;
  }
  this->collisionMesh=this->collisionNormals=nullptr;
}



void PhysicalBody::meshRealloc(Array<VectorXY *> &col, Array<VectorXY *> &nor){
  this->collisionMesh=new Array<VectorXY*> (col);
  this->collisionMesh->map([](VectorXY*& v){v=new VectorXY(*v);});

  this->collisionNormals=new Array<VectorXY*> (nor);
  this->collisionNormals->map([](VectorXY*& v){v=new VectorXY(*v);});
}

void PhysicalBody::reshape(){//reshape to circular object
  this->~PhysicalBody();

  this->collisionMesh=this->collisionNormals=nullptr;
  this->meshType=PhysicalBody::circle;
}

void PhysicalBody::reshape(Array<PointXY *> &mesh){

  int l = mesh.length();
  if (l<3)
    throw "invalid collision mesh";

  this->reshape();
  this->collisionMesh=new Array<VectorXY*> (l);

  PointXY
      *prevPt=mesh.getData(0),
      *currPt;

  for (int i = 0 ; i < l-1 ; i++){
    currPt=mesh.getData(i+1);
    this->collisionMesh->setData(new VectorXY(*prevPt,*currPt),i);
    prevPt=currPt;
  }
  this->collisionMesh->setData(new VectorXY(*currPt,this->collisionMesh->getData(0)->getBegin()),l-1);

  this->computeNormals();
  this->meshType=PhysicalBody::mesh;
}

void PhysicalBody::computeNormals(){

  int l = this->collisionMesh->length();
  this->collisionNormals=new Array<VectorXY*> (l);

  VectorXY * currentVec = this->collisionMesh->getData(0);

  for (int i=0;i<l;i++){
    currentVec = this->collisionMesh->getData(i);
    this->collisionNormals->setData(new VectorXY(
                   PointXY(-currentVec->getBegin().getY(), currentVec->getBegin().getX()), // (a,b)(c,d) → (-b,a)(-d,c)
                   PointXY(-currentVec->getEnd().getY(), currentVec->getEnd().getX())
                   ),i);
  }
}



PhysicalBody& PhysicalBody::operator=(const PhysicalBody& R){
  this->~PhysicalBody();
  if (R.meshType==PhysicalBody::mesh){
    this->meshRealloc(*R.collisionMesh,*R.collisionNormals);
    this->meshType=PhysicalBody::mesh;
  }
  return *this;
}



//////////////////////////////////////////////////////////////////



long long Object::maxId=0;
const Object Object::nullObj=Object();

Object::Object(const std::string name,
               const Decal& decal,
               const PhysicalBody& physicalBody,
               const LightSource& lightSource) :
  PhysicalBody(physicalBody),
  LightSource(lightSource),
  Decal(decal),
  GCRef(),
  name(name),
  id(Object::maxId++)
{
}

Object::Object(const Object &ref) :
  Object(ref.name,ref,ref,ref)
{
}

Object::~Object(){

}
