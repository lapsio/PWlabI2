#include "./include/utils.h"


TypedClass::TypedClass() : typeChain(new Chain<const char*>(TypedClass::typeName)) {}


GCRefLink::GCRefLink(GCRef * ref) : ref(ref) {}

GCRefLink::~GCRefLink(){
  this->ref->unlink();
}

GCRef::GCRef() : refCount(0), lock(true) {}

GCRefLink * GCRef::link(){
  this->lock=false;
  ++this->refCount;
  return new GCRefLink(this);
}

void GCRef::unlink(){
  --this->refCount;
  if(this->refCount==0&&!this->lock)
    delete this;
}

void GCRef::unlock(){
  this->lock=false;
  if (this->refCount==0)
    delete this;
}

int GCRef::refUsersCount() const {
  return this->refCount;
}
