#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <cstring>
#include "./include/PointXY.h"
#include "./include/VectorXY.h"

class Object;
class Decal;
class LightSource;
class PhysicalBody;

class Map;
class ObjectMapMeta;

class PhysicsEngine;
class ObjectPhysicsMeta;

struct GridPool;

template <class TYPE>
class Array {
private:
  TYPE * arr;
  int lengthC;
public:
  Array(int length){this->arr=new TYPE [length];};

  inline int length(){return this->lengthC;};
  TYPE getData(int index){return this->arr[index];};
  void setData(TYPE d, int index){this->arr[index]=d;};

  TYPE& operator[](int index){return this->arr[index];};
};

template <class TYPE>
class Chain {
private:
  Chain * nextC;
  Chain * prevC;
public:
  TYPE data;
  Chain * next(){return this->nextC;};
  Chain * prev(){return this->prevC;};
};



class Decal
{
private:
public:
  Decal(){};
  virtual ~Decal(){};
};

class LightSource
{
private:
public:
  LightSource(){};
  virtual ~LightSource(){};
};

////////////////////////////////////////////

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

public:

  PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, double friction=1, double mass=1);
  PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, Array<PointXY*>& collisionMesh, double friction=1, double mass=1);
  virtual ~PhysicalBody();

  const PointXY& getBound();
  Array<VectorXY*> getCollisionMesh();
  Array<VectorXY*> getCollisionNormals();

  inline ObjectType getObjectType();
  inline CollisionType getCollisionType();
  inline MeshType getMeshType();

  inline double getFriction();
  inline double getMass();

  inline void setObjectType(ObjectType type);
  inline void setCollisionType(CollisionType collision);
  inline void setMass(double mass);
  inline void setFriction(double friction);
};



class Object : public PhysicalBody, public LightSource, public Decal
{
private:
  static int maxId;
  const std::string name;
  int id;
public:
  Object(const std::string name = std::string("Object"),
         const Decal& = Decal(),
         const PhysicalBody& = PhysicalBody(PhysicalBody::passive, PhysicalBody::ghost, PointXY(0,0)),
         const LightSource& = LightSource());
  Object(const Object& ref);
  virtual ~Object();

  inline virtual const char * typeOf(){return "Object";};
  inline bool operator==(const Object& o){return o.id==this->id;}
  inline static bool matchType(Object obj){return strcmp(obj.typeOf(),"Object")==0;};
};



//////////////////////////////////////////////



class ObjectMapMeta {
public:
  PointXY pos;
  Object& object;

  ObjectMapMeta(Object& obj);
  virtual ~ObjectMapMeta();
};

class GameMap {
private:
  Chain<Object> objects;
public:
  GameMap();
  ~GameMap();

  void addObject(Object& obj);
  void deleteObject(Object* obj);
  void deleteObject(int index);
};

/////////////////////////////////////////////

class ObjectPhysicsMeta : public ObjectMapMeta{
private:
  PhysicsEngine * engine;
public:
  VectorXY speed;
  VectorXY acceleration;

  Chain<struct GridPool> * gridChain;
  Object * anchor;

  ObjectPhysicsMeta(Object& obj);
  virtual ~ObjectPhysicsMeta();
};

struct GridPool {
  int posX, posY, indR, indC;
  Chain<Object> * objectsChain; //(Object *);
};

class CollisionGrid {
public:
  int gridC,gridR,gridW,gridH;
  struct GridPool ** GridPool;

  CollisionGrid(GameMap* map);
  ~CollisionGrid();

  void reload();
  Chain<Object> * getObjects(int R, int C);
};

class PhysicsEngine
{
private:
  GameMap* map;
  CollisionGrid* collisionGrid;

  void moveObjects(int& timeShift, int objIndex=-1);
  void moveAnchors(int& timeShift, int objIndex=-1);
  void collideObjects(int& timeShift, int objIndex=-1);
  void postMotion(int& timeShift, int objIndex=-1);

public:
  PhysicsEngine(GameMap* map);
  ~PhysicsEngine();

  void reloadMap(GameMap * map);

  void registerObject(Object *);
  void removeObject();
  int timeShift();
};

#endif // OBJECT_H
