#ifndef PHYSX_H
#define PHYSX_H

#include "./include/defines.h"

#include "./include/map.h"


class Timer;
class PhysicsEngine;
class ObjectPhysicsMeta;


class Timer {
private:
  unsigned long long lastTime;
  long double bufferedTimeShift;
  bool paused;

public:
  const int freq;

  Timer(int frequency=25);
  ~Timer(){}

  bool shift(int diff);
  void pause();
  void resume();
  inline bool isPaused(){return this->paused;}

};


class PhysicsEngine
{
public:
  class CollisionGrid {
    friend class PhysicsEngine;
  public:
    struct GridPool {
      int posX, posY, indR, indC;
      Chain<ObjectPhysicsMeta*> * objectsChain; //(Object *);
    };

  private:
    struct GridPool *** grid;
    GameMap* map;

    struct whp {int a;int b;};
    whp computeDim(GameMap* map);

  public:
    const int gridW,gridH,gridC,gridR;

    CollisionGrid(GameMap& map);
    ~CollisionGrid();

    void registerMeta(ObjectPhysicsMeta& meta);
    void eraseMetaRecords(ObjectPhysicsMeta* meta);

    static VectorXY getObjectBounds(ObjectPhysicsMeta& m);
  };

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



class ObjectPhysicsMeta : public ObjectMapMeta {
  friend class PhysicsEngine::CollisionGrid;
public:
  static constexpr char* typeName=(char*)"PhysicsMeta";

private:
  const PhysicsEngine * engine;
  Chain<PhysicsEngine::CollisionGrid::GridPool*> * gridNeighbours;

protected:
  virtual void linkMap(GameMap &map);
  virtual void unlinkMap();

public:
  VectorXY speed;
  VectorXY acceleration;

  ObjectPhysicsMeta(const ObjectMapMeta& ref, const PhysicsEngine& engine, VectorXY speed=VectorXY(0,0,0,0), VectorXY acceleration=VectorXY(0,0,0,0));
  virtual ~ObjectPhysicsMeta();
};


#endif // PHYSX_H
