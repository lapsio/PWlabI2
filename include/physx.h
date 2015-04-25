#ifndef PHYSX_H
#define PHYSX_H

#include "./include/defines.h"

#include "./include/map.h"



class PhysicsEngine;
class ObjectPhysicsMeta;

struct GridPool;



class ObjectPhysicsMeta : public ObjectMapMeta{
public:
  static constexpr char* typeName=(char*)"PhysicsMeta";

private:
  const PhysicsEngine * engine;

protected:
  virtual void linkMap(GameMap &map);
  virtual void unlinkMap();

public:
  VectorXY speed;
  VectorXY acceleration;

  Chain<struct GridPool> * gridChain;

  ObjectPhysicsMeta(const ObjectMapMeta& ref, const PhysicsEngine& engine, VectorXY speed=VectorXY(0,0,0,0), VectorXY acceleration=VectorXY(0,0,0,0));
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

  int getTimeShift(int objIndex=-1);
  void moveObjects(int& timeShift, int objIndex=-1);
  void moveAnchors(int& timeShift, int objIndex=-1);
  void collideObjects(int& timeShift, int objIndex=-1);
  void postMotion(int& timeShift, int objIndex=-1);

public:
  PhysicsEngine(GameMap& map);
  ~PhysicsEngine();

  inline GameMap* getMap() const {return this->map;}

  void reloadMap(GameMap& map);

  void registerObject(Object *);
  void removeObject();
  int timeShift();
};


#endif // PHYSX_H
