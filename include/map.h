#ifndef MAP_H
#define MAP_H

#include "./include/object.h"



class GameMap;
class ObjectMapMeta;



class ObjectMapMeta : public GCRef{
  friend class GameMap;

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

public:
  Object& object;
  PointXY pos;

  ObjectMapMeta(Object& obj,PointXY pos=PointXY(0,0));
  ObjectMapMeta(const ObjectMapMeta& ref);
  virtual ~ObjectMapMeta();

  inline ObjectMapMeta* getAnchor(){return this->anchor;}
  inline ObjectMapMeta* getTail(){return this->tail;}

  void attach(ObjectMapMeta& anchor);
  void detach();

  inline bool isInUse(){return this->inUse;}
  inline GameMap* getMap(){return this->map;}

private:
  void operator=(const ObjectMapMeta& R){(void)R;}
};



class GameMap {
private:
  Chain<ObjectMapMeta&> * objects;
public:
  GameMap();
  ~GameMap();

  int getIndex(ObjectMapMeta * meta);
  ObjectMapMeta& getMeta(int index);

  void addObject(Object& obj,PointXY pos=PointXY(0,0));
  void addObject(ObjectMapMeta& meta);
  ObjectMapMeta& popObject(ObjectMapMeta * meta);
  ObjectMapMeta& popObject(int index);
  void deleteObject(ObjectMapMeta * meta);
  void deleteObject(int index);
};


#endif // MAP_H
