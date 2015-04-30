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
  bool autosleep;

public:
  const unsigned int freq;

  Timer(int frequency=25, bool autosleep=true);
  ~Timer(){}

  bool shift(int diff);
  void pause();
  void resume();
  inline void setAutosleep(bool value){this->autosleep=value;}
  inline void toggleAutosleep(){this->autosleep=!this->autosleep;}
  inline bool isPaused(){return this->paused;}
  inline bool isAutoSleep(){return this->autosleep;}

};


class PhysicsEngine
{
public:
  struct CREnt {
    ObjectPhysicsMeta*A;
    ObjectPhysicsMeta*B;
  };

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

    static Chain<PhysicsEngine::CollisionGrid::GridPool*>* getNeighbours(ObjectPhysicsMeta& m);
    GridPool * getPool(int C, int R){
      if(C>-1&&R>-1
         &&C<this->gridC&&R<this->gridR)
        return this->grid[C][R];
      throw "Invalid index";}

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

  Chain<CREnt>* collisionsRegistry;
  long double timeScale;

  int getTimeShift(int objIndex=-1);
  void moveObjects(int& timeShift, int objIndex=-1);
  void collideObjects(int& timeShift, int objIndex=-1);
  void postMotion(int& timeShift, int objIndex=-1);

  ObjectPhysicsMeta * getObjectCollisions(ObjectPhysicsMeta& meta, const bool& collide=true);
  int isColliding(ObjectPhysicsMeta& A, ObjectPhysicsMeta& B, const bool& collide=false);

  void loadMapData(GameMap& map);
  void purgeMapData();

public:
  PhysicsEngine(GameMap& map);
  ~PhysicsEngine();

  inline GameMap* getMap() const {return this->map;}
  inline Chain<CREnt>* getCollisionRegistry() const {return this->collisionsRegistry;}

  void reloadMap(GameMap& map);

  void registerObject(ObjectMapMeta& meta);
  void unregisterObject(ObjectPhysicsMeta& meta);
  int timeShift();

#ifdef _DEBUG
  static void DEBUG(PhysicsEngine * p);
#endif
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

#ifdef _DEBUG
public:
  static void DEBUG(ObjectPhysicsMeta * m){
    ObjectMapMeta::DEBUG(m);
    std::cout << "engine: " << m->engine  << std::endl;
    std::cout << "speed: (" <<  m->speed.getBegin().X << "," << m->speed.getBegin().Y << ") (" << m->speed.getEnd().X << ";" << m->speed.getEnd().Y << ")" << std::endl;
    std::cout << "acceleration: (" <<  m->acceleration.getBegin().X << "," << m->acceleration.getBegin().Y << ") (" << m->acceleration.getEnd().X << ";" << m->acceleration.getEnd().Y << ")" << std::endl;

    int i=0;
    Chain<PhysicsEngine::CollisionGrid::GridPool*>*c=m->gridNeighbours;
    while((c=c->next())){
      ++i;
      int j=0;
      Chain<ObjectPhysicsMeta*>*cc=c->data->objectsChain;
      while((cc=cc->next())){
        ++j;
        std::cout << "pool:" << i << ", object:" << j << " - " << cc->data->object.getName() << std::endl;
      }
    }
    std::cout << std::endl;
  }
#endif
};


#endif // PHYSX_H
