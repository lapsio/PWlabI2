#include "./include/map.h"



ObjectMapMeta::ObjectMapMeta(Object &obj, PointXY pos) :
  inUse(false),
  mapLink(nullptr),
  map(nullptr),
  anchor(nullptr),
  tail(nullptr),
  objLink(*obj.link()),
  object(obj),
  pos(pos)
{
}

ObjectMapMeta::ObjectMapMeta(const ObjectMapMeta &ref) :
  inUse(false),
  mapLink(nullptr),
  map(nullptr),
  anchor(nullptr),
  tail(nullptr),
  objLink(*ref.object.link()),
  object(ref.object),
  pos(ref.pos)
{
}

ObjectMapMeta::~ObjectMapMeta(){
  this->map=nullptr;
  this->inUse=false;
  this->detach();
  if (this->tail)
    this->tail->detach();

  delete &this->objLink;
}

void ObjectMapMeta::attach(ObjectMapMeta &anchor){
  this->detach();
  this->anchor=&anchor;
  if (this->anchor->tail)
    this->anchor->tail->detach();
  this->anchor->tail=this;
}

void ObjectMapMeta::detach(){
  if (this->anchor)
    this->anchor->tail=nullptr;
  this->anchor=nullptr;
}

void ObjectMapMeta::linkMap(GameMap& map){
  this->inUse=true;
  this->map=&map;
  this->mapLink=this->link();
}

void ObjectMapMeta::unlinkMap(){
  this->inUse=false;
  this->map=nullptr;
  delete this->mapLink;
}


///////////////////////////////////////////////////////////////////


GameMap::GameMap() :
  objects(new Chain<ObjectMapMeta&>(*(new ObjectMapMeta(*(new Object(Object::nullObj))))))
{
  this->objects->data.inUse=true;
  this->objects->data.map=this;
  this->objects->data.mapLink=this->objects->data.link();
}

GameMap::~GameMap(){
  this->objects->map([](ObjectMapMeta& m){m.unlinkMap();});
  delete this->objects;
}



void GameMap::addObject(Object &obj, PointXY pos){
  this->addObject(*(new ObjectMapMeta(obj,pos)));
}

void GameMap::addObject(ObjectMapMeta &meta){
  meta.linkMap(*this);
  this->objects->insertAfter(meta);
}



int GameMap::getIndex(ObjectMapMeta *meta){
  Chain<ObjectMapMeta&>*c=this->objects;
  int index=0;
  while(1){
    if (&c->data==meta)
      break;
    index++;
    if (!(c=c->next()))
      return -1;
  }
  return index;
}

ObjectMapMeta& GameMap::getMeta(int index){
  Chain<ObjectMapMeta&>*c=this->objects;
  while(index-->0&&c)
    c=c->next();

  if (!c||index==-2)//do NOT allow to pop dummyObject
    throw "out of index - map entry not found";

  return c->data;
}



ObjectMapMeta& GameMap::popObject(ObjectMapMeta *meta){
  return this->popObject(this->getIndex(meta));
}

ObjectMapMeta& GameMap::popObject(int index){
  Chain<ObjectMapMeta&>*c=this->objects;
  while(index-->0&&c)
    c=c->next();

  if (!c||index==-2)//do NOT allow to pop dummyObject
    throw "out of index - map entry not found";

  ObjectMapMeta& m = c->data;

  if (m.refUsersCount()==1)
    throw "cannot pop without destroying meta - map is the only user of object.";

  c->data.unlinkMap();
  delete c;

  return m;
}



void GameMap::deleteObject(ObjectMapMeta * meta){
  this->deleteObject(this->getIndex(meta));
}

void GameMap::deleteObject(int index){
  Chain<ObjectMapMeta&>*c=this->objects;
  while(index-->0&&c)
    c=c->next();

  if (!c||index==-2)
    throw "out of index - map entry not found";

  c->data.unlinkMap();
  delete c;
}
