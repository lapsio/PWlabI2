#ifndef MAP_H
#define MAP_H

#include "./include/defines.h"

#include "./include/object.h"



class GameMap;
class ObjectMapMeta;


/**
 * @brief The ObjectMapMeta class Object meta class containing map-related properties of object.
 *
 * ObjectMapMeta class provides map-specific properties related to object like various relations between objects (attachment of one object to another) and position.
 * Single Object can be placed multiple times on single map using multiple ObjectMapMeta instances, however it'll literally link instances of object on map,
 * thus any operations performed on original object except map-specific properties will affect both of them.
 * Useful for template and non-interactive objects which can be placed in many places on single map
 *
 * @see GameMap
 * @see ObjectPhysicsMeta
 */
class ObjectMapMeta : public GCRef, public TypedClass{
  friend class GameMap;

public:
  static constexpr char* typeName=(char*)"Meta"; /**< type name */

protected:
  bool inUse;
  GCRefLink* mapLink;
  GameMap * map;

  virtual void linkMap(GameMap& map);//implies adding/removing meta to map
  virtual void unlinkMap();

private:
  ObjectMapMeta * anchor;
  ObjectMapMeta * tail;
  GCRefLink& objLink;

  bool detectAnchorLoop(const ObjectMapMeta* candidate) const;

public:
  Object& object; /**< represented object reference */
  PointXY pos; /**< object position or offset from anchor (parent)*/

  /**
   * @brief ObjectMapMeta Constructor
   * @param obj represented object
   * @param pos initial position
   */
  ObjectMapMeta(Object& obj,PointXY pos=PointXY(0,0));

  /**
   * @brief ObjectMapMeta copy constructor
   * @param ref reference meta
   */
  ObjectMapMeta(const ObjectMapMeta& ref);
  virtual ~ObjectMapMeta();

  /**
   * @brief getAnchor returns parent object which meta is attached to
   * @return parent meta
   */
  inline ObjectMapMeta* getAnchor() const {return this->anchor;}
  /**
   * @brief getTail returns child object which is attached to this object
   * @return child meta
   */
  inline ObjectMapMeta* getTail() const {return this->tail;}
  /**
   * @brief getRoot returns topmost parent in objects hierarchy
   * @return topmost parent meta
   */
  inline ObjectMapMeta* getRoot() const {ObjectMapMeta* k=const_cast<ObjectMapMeta*>(this);while(k->anchor)k=k->anchor;return k;}
  /**
   * @brief getGlobalPos returns calculated global position (affected by
   * @return
   */
  inline PointXY getGlobalPos() const {ObjectMapMeta* m=const_cast<ObjectMapMeta*>(this->getRoot());PointXY p(0,0);while(m){p+=m->pos;if(m==this)return p;m=m->tail;}}

  /**
   * @brief attach Attaches object to another one
   * @param anchor new parent meta
   */
  void attach(ObjectMapMeta& anchor);
  /**
   * @brief detach Detaches object from parent
   */
  void detach();

  /**
   * @brief isInUse Indicates whether meta is already placed on map or not
   * @return
   */
  inline bool isInUse() const {return this->inUse;}
  /**
   * @brief getMap returns map which object is placed on
   * @return
   */
  inline GameMap* getMap() const {return this->map;}

private:
  void operator=(const ObjectMapMeta& R){(void)R;}

#ifdef _DEBUG
public:
  static void DEBUG(ObjectMapMeta * m){
    std::cout << "type: " << m->typeOf() << std::endl;

    std::cout << "SELF: " << m << std::endl;

    std::cout << "inUse: " << m->inUse << std::endl;
    std::cout << "mapLink: " << m->mapLink << std::endl;
    std::cout << "map: " << m->map << std::endl;

    std::cout << "anchor: " << m->anchor << std::endl;
    std::cout << "tail: " << m->tail << std::endl;

    std::cout << "Obj pointer: " << &m->object << std::endl;
    std::cout << "refLink: " << &m->objLink << std::endl;

    std::cout << "pos:" << m->pos.getX() << " " << m->pos.getY() << std::endl;
  }

#endif
};


/**
 * @brief The GameMap class Map class.
 * Contains objects.
 *
 * @see ObjectMapMeta
 * @see PhysicsEngine
 */
class GameMap {
private:
  Chain<ObjectMapMeta&> * objects;
public:
  const int width; /**< map width */
  const int height; /**< map height */

  /**
   * @brief GameMap Map constructor
   * @param width map width
   * @param heigth map height
   */
  GameMap(int width = 64, int heigth = 64);
  ~GameMap();

  /**
   * @brief getIndex searches for specified meta in map objects list and returns index
   * @param meta meta to be looked for
   * @return index in map objects list
   */
  int getIndex(const ObjectMapMeta * meta) const;
  /**
   * @brief getMeta returns object meta from given position in objects list
   * @param index index of requested meta
   * @return object meta
   */
  ObjectMapMeta& getMeta(int index) const;

  /**
   * @brief addObject Places object on map
   * @param obj object to be placed
   * @param pos initial position
   */
  void addObject(Object& obj,PointXY pos=PointXY(0,0));
  /**
   * @brief addObject Places meta on map
   * @param meta to be placed
   */
  void addObject(ObjectMapMeta& meta);
  /**
   * @brief popObject removes meta from map (specified by meta)
   * @param meta meta to be removed
   * @return
   */
  ObjectMapMeta& popObject(ObjectMapMeta * meta);
  /**
   * @brief popObject removes meta from map (specified by index)
   * @param index index of meta to be removed
   * @return
   */
  ObjectMapMeta& popObject(int index);
  /**
   * @brief deleteObject deletes meta (specified by meta)
   * @param meta meta to be deleted
   */
  void deleteObject(ObjectMapMeta * meta);
  /**
   * @brief deleteObject deletes meta (specified by index)
   * @param index index of meta to be deleted
   */
  void deleteObject(int index);

  /**
   * @brief clear Clears map from all objects
   */
  void clear();

  /**
   * @brief operator [] table operator for map. Returns object under specified index on list
   * @param index index of meta to be returned
   * @return
   *
   * @see GameMap::getMeta
   */
  ObjectMapMeta& operator[](const int& index) const {return this->getMeta(index);}
  int operator[](const ObjectMapMeta& m) const {return this->getIndex(&m);}
  inline int length(){int l=0;Chain<ObjectMapMeta&>*c=this->objects;while((c=c->next()))++l;return ++l;}

#ifdef _DEBUG
  static void DEBUG(GameMap* a){
    Chain<ObjectMapMeta&>*c=a->objects;
    int k=0;
    while(c){
      std::cout << std::endl << "#" << k++ << ":" << std::endl;
      ObjectMapMeta::DEBUG(&c->data);
      Object::DEBUG(&c->data.object);
      c=c->next();
    }
  }
#endif
};


#endif // MAP_H
