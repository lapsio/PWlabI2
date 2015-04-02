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

#ifdef _DEBUG
public:
  static void DEBUG(ObjectMapMeta * m){
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
