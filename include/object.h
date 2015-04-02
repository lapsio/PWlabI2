#ifndef OBJECT_H
#define OBJECT_H

#include "./include/defines.h"

#include <string>
#include <cstring>
#include "./include/PointXY.h"
#include "./include/VectorXY.h"
#include "./include/misc.h"
#include "./include/gc.h"



class Object;
class Decal;
class LightSource;
class PhysicalBody;



class Decal
{
private:
public:
  Decal(){}
  virtual ~Decal(){}
};

class LightSource
{
private:
public:
  LightSource(){}
  virtual ~LightSource(){}
};



class PhysicalBody
{
  friend class PhysicsEngine;

public:
  enum ObjectType {passive='p', dynamic='d'};
  enum CollisionType {ghost='g',solid='s'};
  enum MeshType {circle='c', mesh='m'};

private:
  PointXY boundBox;
  Array<VectorXY*> *collisionMesh, *collisionNormals;

  ObjectType type;
  CollisionType collisionType;
  MeshType meshType;

  double friction, mass;

  void computeNormals();
  void meshRealloc(Array<VectorXY*>& col, Array<VectorXY*>& nor);

public:

  PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, double friction=1, double mass=1);
  PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, Array<PointXY*>& collisionMesh, double friction=1, double mass=1);
  PhysicalBody(const PhysicalBody& ref);
  virtual ~PhysicalBody();

  const PointXY& getBound() const {return this->boundBox;}
  const Array<VectorXY*>& getCollisionMesh() const {return *((this)->collisionMesh);}
  const Array<VectorXY*>& getCollisionNormals() const {return *((this)->collisionNormals);}

  inline ObjectType getObjectType() const {return this->type;}
  inline CollisionType getCollisionType() const {return this->collisionType;}
  inline MeshType getMeshType() const {return this->meshType;}

  inline double getFriction() const {return this->friction;}
  inline double getMass() const {return this->mass;}

  void reshape();
  void reshape(Array<PointXY*>& collisionMesh);

  inline void setObjectType(ObjectType type){this->type=type;}
  inline void setCollisionType(CollisionType collision){this->collisionType=collision;}
  inline void setMass(double mass){mass>0?this->mass=mass:throw "mass must be positive value";}
  inline void setFriction(double friction){friction>0?this->friction=friction:throw "friction must be positive value";}

  PhysicalBody& operator=(const PhysicalBody& R);

#ifdef _DEBUG
  static void DEBUG(PhysicalBody * o){
    std::cout << "bounds: " << o->boundBox.getX() << " " << o->boundBox.getY() << std::endl;
    std::cout << "flags: " << (char)o->type << " " << (char)o->collisionType << " " << (char)o->meshType << std::endl;
    std::cout << "props: mass: " << o->mass << " friction: " << o->friction << std::endl;
    if (o->meshType==PhysicalBody::mesh){
      std::cout << "mesh: " << std::endl;

      int l = o->collisionMesh->length();
      for (int i = 0 ; i < l ; i++)
        std::cout
            << "   " << o->collisionMesh->getData(i)->getBegin().getX()
            << ":" << o->collisionMesh->getData(i)->getBegin().getY()
            << "   " << o->collisionMesh->getData(i)->getEnd().getX()
            << ":" << o->collisionMesh->getData(i)->getEnd().getY() << std::endl;

      std::cout << "normals: " << std::endl;

      l = o->collisionNormals->length();
      for (int i = 0 ; i < l ; i++)
        std::cout
            << "   " << o->collisionNormals->getData(i)->getBegin().getX()
            << ":" << o->collisionNormals->getData(i)->getBegin().getY()
            << "   " << o->collisionNormals->getData(i)->getEnd().getX()
            << ":" << o->collisionNormals->getData(i)->getEnd().getY() << std::endl;
    } else
      std::cout << "no mesh - circular object" << std::endl << std::endl;
  }
#endif
};



class Object : public PhysicalBody, public LightSource, public Decal, public GCRef
{
private:
  const std::string name;
  const long long id;

  static long long maxId;
public:
  static const Object nullObj;

  Object(const std::string name = std::string("Object"),
         const Decal& = Decal(),
         const PhysicalBody& = PhysicalBody(PhysicalBody::passive, PhysicalBody::ghost, PointXY(0,0)),
         const LightSource& = LightSource());
  Object(const Object& ref);
  virtual ~Object();

  inline const std::string getName() const {return this->name;}
  inline virtual const char * typeOf() const {return "Object";}
  inline bool operator==(const Object& o) const {return o.id==this->id;}
  inline static bool matchType(Object obj){return strcmp(obj.typeOf(),"Object")==0;}

#ifdef _DEBUG
  static void DEBUG(Object * o){
    std::cout << "SELF: " << o << std::endl;

    std::cout << "name: " << o->name << std::endl;
    std::cout << "id:" << o->id << std::endl;
    std::cout << "maxId:" << Object::maxId << std::endl;

    std::cout << "refUsers:" << o->refUsersCount() << std::endl;
  }
#endif
};


#endif // OBJECT_H
