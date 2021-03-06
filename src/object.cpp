#include "./include/object.h"
#include "./include/utils.h"

Decal::Decal(const char *filePath, PointXY offset) :
  texture(this->loadTexture(filePath)),
  offset(offset)
{

}

Decal::~Decal(){
  //delete this->texture;
}

const sf::Texture* Decal::loadTexture(const char *path){
  sf::Texture* tx = new sf::Texture();

  char buff[256];
  size_t s = snprintf(buff, 256,"./assets/textures/%s", path);
  buff[s]=0;

  if (!tx->loadFromFile(buff))
    throw "Could not load image";

  return (const_cast<const sf::Texture*>(tx));
}

PhysicalBody::PhysicalBody(ObjectType type,CollisionType collision, PointXY dimensions,double friction,double mass){
  if (dimensions.getX()<0||dimensions.getY()<0)
    throw "dimensions cannot be negative";

  this->boundBox=dimensions;
  this->setMass(mass);
  this->setFriction(friction);

  this->setObjectType(type);
  this->setCollisionType(collision);

  this->meshType=PhysicalBody::MeshType::circle;
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
  if (ref.meshType==PhysicalBody::MeshType::mesh){
    this->meshRealloc(*ref.collisionMesh,*ref.collisionNormals);
    this->meshType=PhysicalBody::MeshType::mesh;
  }
}

PhysicalBody::~PhysicalBody(){
  this->reshape();
}



void PhysicalBody::meshRealloc(Array<VectorXY *> &col, Array<VectorXY *> &nor){
  this->collisionMesh=new Array<VectorXY*> (col);
  this->collisionMesh->map([](VectorXY*& v){v=new VectorXY(*v);});

  this->collisionNormals=new Array<VectorXY*> (nor);
  this->collisionNormals->map([](VectorXY*& v){v=new VectorXY(*v);});
}

void PhysicalBody::reshape(){//reshape to circular object
  if (this->meshType==PhysicalBody::MeshType::mesh){
    this->collisionMesh->map([](VectorXY*& v){delete v;v=nullptr;});
    delete this->collisionMesh;
    this->collisionNormals->map([](VectorXY*& v){delete v;v=nullptr;});
    delete this->collisionNormals;
  }

  this->collisionMesh=this->collisionNormals=nullptr;
  this->meshType=PhysicalBody::MeshType::circle;
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
  this->meshType=PhysicalBody::MeshType::mesh;
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
  if (R.meshType==PhysicalBody::MeshType::mesh){
    this->meshRealloc(*R.collisionMesh,*R.collisionNormals);
    this->meshType=PhysicalBody::MeshType::mesh;
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
  this->pushType(Object::typeName);
}

Object::Object(const Object &ref) :
  Object(ref.name,ref,ref,ref)
{
}

Object::~Object(){

}

std::ostream& operator<<(std::ostream& os, const Object& o)
{
    os << o.id << "/" << o.maxId << " (" << o.refUsersCount() << "):" << o.typeOf() << "," << o.name;
    return os;
}
