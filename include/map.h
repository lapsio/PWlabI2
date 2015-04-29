#ifndef MAP_H
#define MAP_H

#include "./include/defines.h"

#include "./include/object.h"



class GameMap;
class ObjectMapMeta;



class ObjectMapMeta : public GCRef, public TypedClass{
  friend class GameMap;

public:
  static constexpr char* typeName=(char*)"Meta";

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
  Object& object;
  PointXY pos;

  ObjectMapMeta(Object& obj,PointXY pos=PointXY(0,0));
  ObjectMapMeta(const ObjectMapMeta& ref);
  virtual ~ObjectMapMeta();

  inline ObjectMapMeta* getAnchor() const {return this->anchor;}
  inline ObjectMapMeta* getTail() const {return this->tail;}
  inline ObjectMapMeta* getRoot() const {ObjectMapMeta* k=const_cast<ObjectMapMeta*>(this);while(k->anchor)k=k->anchor;return k;}
  inline PointXY getGlobalPos() const {ObjectMapMeta* m=const_cast<ObjectMapMeta*>(this->getRoot());PointXY p(0,0);while(m){p+=m->pos;if(m==this)return p;m=m->tail;}}

  void attach(ObjectMapMeta& anchor);
  void detach();

  inline bool isInUse() const {return this->inUse;}
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



class GameMap {
private:
  Chain<ObjectMapMeta&> * objects;
public:
  const int width;
  const int height;

  GameMap(int width = 64, int heigth = 64);
  ~GameMap();

  int getIndex(const ObjectMapMeta * meta) const;
  ObjectMapMeta& getMeta(int index) const;

  void addObject(Object& obj,PointXY pos=PointXY(0,0));
  void addObject(ObjectMapMeta& meta);
  ObjectMapMeta& popObject(ObjectMapMeta * meta);
  ObjectMapMeta& popObject(int index);
  void deleteObject(ObjectMapMeta * meta);
  void deleteObject(int index);

  void clear();

  inline ObjectMapMeta& operator[](const int& index) const {return this->getMeta(index);}
  inline int operator[](const ObjectMapMeta& m) const {return this->getIndex(&m);}
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
