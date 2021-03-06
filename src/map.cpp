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
  this->pushType(ObjectMapMeta::typeName);
}

ObjectMapMeta::ObjectMapMeta(const ObjectMapMeta &ref) :
  ObjectMapMeta(ref.object,ref.pos)
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

bool ObjectMapMeta::detectAnchorLoop(const ObjectMapMeta *candidate) const {
  ObjectMapMeta* root=const_cast<ObjectMapMeta*>(this->getRoot());
  while(root){
    if(root==candidate)return true;
    root=root->tail;}
  return false;
}

void ObjectMapMeta::attach(ObjectMapMeta &anchor){
  if (this->detectAnchorLoop(&anchor))
    throw "chain loop detected";

  this->detach();
  this->anchor=&anchor;
  if (this->anchor->tail)
    this->anchor->tail->detach();
  this->anchor->tail=this;
  this->pos-=this->anchor->pos;//in attached objects pos becomes offset from anchor
}

void ObjectMapMeta::detach(){
  if (!this->anchor)
    return;
  this->pos+=this->anchor->pos;//make pos global;
  this->anchor->tail=nullptr;
  this->anchor=nullptr;
}

void ObjectMapMeta::linkMap(GameMap& map){
  if (this->inUse)
    throw "Meta already in use";
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


GameMap::GameMap(int width, int height) :
  objects(new Chain<ObjectMapMeta&>(*(new ObjectMapMeta(*(new Object(Object::nullObj)))))),
  width(width),
  height(height)
{
  this->objects->data.inUse=true;
  this->objects->data.map=this;
  this->objects->data.mapLink=this->objects->data.link();
}

GameMap::~GameMap(){
  this->clear();
  this->objects->data.unlinkMap();
  delete this->objects;
}

void GameMap::addObject(Object &obj, PointXY pos){
  this->addObject(*(new ObjectMapMeta(obj,pos)));
}

void GameMap::addObject(ObjectMapMeta &meta){
  meta.linkMap(*this);
  this->objects->insertAfter(meta);
}



int GameMap::getIndex(const ObjectMapMeta *meta) const {
  Chain<ObjectMapMeta&>*c=this->objects->next();//do not allow getting dummyObject
  int index=1;
  while(1){
    if (&c->data==meta)
      break;
    index++;
    if (!(c=c->next()))
      return -1;
  }
  return index;
}

ObjectMapMeta& GameMap::getMeta(int index) const {
  Chain<ObjectMapMeta&>*c=this->objects->next();
  while(index-->1&&c)
    c=c->next();

  if (!c||index==-1)//do NOT allow to pop dummyObject
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

void GameMap::clear(){
  while(this->objects->next())
    this->deleteObject(1);
}
