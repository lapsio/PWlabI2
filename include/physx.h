#ifndef PHYSX_H
#define PHYSX_H

#include "./include/defines.h"

#include "./include/map.h"


class Timer;
class PhysicsEngine;
class ObjectPhysicsMeta;


/**
 * @brief The Timer class Timer class providing timer functionality, can be used to generate signal with certain frequency
 */
class Timer {
private:
  unsigned long long lastTime;
  long double bufferedTimeShift;
  bool paused;
  bool autosleep;

public:
  const unsigned int freq; /**< timer base frequency */

  /**
   * @brief Timer constructor
   * @param frequency requested timer frequency
   * @param autosleep whether timer should call sleep function and pause code execution till next expected tick or not
   */
  Timer(int frequency=25, bool autosleep=true);
  ~Timer(){}

  /**
   * @brief shift record time shift
   * @param diff time shift to be performed (in miliseconds)
   * @return
   */
  bool shift(int diff);
  /**
   * @brief pause pause timer
   */
  void pause();
  /**
   * @brief resume resume timer
   */
  void resume();
  /**
   * @brief setAutosleep change autosleep flag value
   * @param value new value
   */
  inline void setAutosleep(bool value){this->autosleep=value;}
  /**
   * @brief toggleAutosleep toggle autosleep flag value
   */
  inline void toggleAutosleep(){this->autosleep=!this->autosleep;}
  /**
   * @brief isPaused indicates whether timer is paused or not
   * @return yup or nope
   */
  inline bool isPaused(){return this->paused;}
  /**
   * @brief isAutoSleep indicates whether timer uses autosleep or not
   * @return yup or nope
   */
  inline bool isAutoSleep(){return this->autosleep;}

};


/**
 * @brief The PhysicsEngine class Class providing basic physics functionality to registered map
 *
 * @see GameMap
 * @see ObjectPhysicsMeta
 */
class PhysicsEngine
{
public:
  struct CREnt { /**< collision registry entry struct */
    ObjectPhysicsMeta*A;
    ObjectPhysicsMeta*B;
  };

  /**
   * @brief The CollisionGrid class Class containing collision grid for optimization purpose
   */
  class CollisionGrid {
    friend class PhysicsEngine;
  public: /**< grid pool entry */
    struct GridPool {
      int posX, posY, indR, indC;
      Chain<ObjectPhysicsMeta*> * objectsChain; /**< list of objects in single sector */
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
    const int
    gridW, /**< width of single sector */
    gridH, /**< height of single sector */
    gridC, /**< number of columns in sectors grid */
    gridR; /**< number of rows in sectors grid */

    /**
     * @brief CollisionGrid Constructor
     * @param map map to create grid for
     */
    CollisionGrid(GameMap& map);
    ~CollisionGrid();

    /**
     * @brief registerMeta register meta in collision grid
     * @param meta meta to be registered
     */
    void registerMeta(ObjectPhysicsMeta& meta);
    /**
     * @brief eraseMetaRecords remove meta from collision grid
     * @param meta meta to be removed
     */
    void eraseMetaRecords(ObjectPhysicsMeta* meta);

    /**
     * @brief getObjectBounds returns bounding box occupied by object
     * @param m meta of requested object
     * @return pair top left corner : bottom right corner
     */
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
  /**
   * @brief PhysicsEngine Constructor
   * @param map map to register physics engine for
   */
  PhysicsEngine(GameMap& map);
  ~PhysicsEngine();

  /**
   * @brief getMap return map registered by engine
   * @return map registered by engine
   */
  inline GameMap* getMap() const {return this->map;}
  /**
   * @brief getCollisionRegistry returns list of collisions
   * @return list of collisions
   */
  inline Chain<CREnt>* getCollisionRegistry() const {return this->collisionsRegistry;}

  /**
   * @brief reloadMap bind engine to different map
   * @param map new map to be bound to
   */
  void reloadMap(GameMap& map);

  /**
   * @brief registerObject register object in physics engine
   * @param meta meta to be registered
   */
  void registerObject(ObjectMapMeta& meta);
  /**
   * @brief unregisterObject unregister object from physics engine
   * @param meta meta to be unregistered
   */
  void unregisterObject(ObjectPhysicsMeta& meta);
  /**
   * @brief timeShift perform physics clock tick
   * @return time shifted by engine
   */
  int timeShift();

#ifdef _DEBUG
  static void DEBUG(PhysicsEngine * p);
#endif
};


/**
 * @brief The ObjectPhysicsMeta class Extension to ObjectMapMeta class adding physical properties like speed, acceleration...
 *
 * @see ObjectMapMeta
 * @see PhysicsEngine
 */
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
  VectorXY speed; /**< object speed */
  VectorXY acceleration; /**< object acceleration */

  /**
   * @brief ObjectPhysicsMeta Constructor
   * @param ref reference meta to be converted to Physics meta
   * @param engine engine to register in
   * @param speed initial speed
   * @param acceleration initial acceleration
   */
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
