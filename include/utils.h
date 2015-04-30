#ifndef GC_H
#define GC_H

#include "./include/defines.h"
#include "./include/misc.h"

class TypedClass;
class GCRefLink;
class GCRef;


class TypedClass
{
private:
  Chain<const char*>* typeChain;

protected:
  inline void pushType(const char * type){this->typeChain->insertBefore(type);this->typeChain=this->typeChain->prev();}

public:
  static constexpr char * typeName=(char*)"Generic";

  TypedClass();
  virtual ~TypedClass(){while(this->typeChain->next())delete this->typeChain->next();delete this->typeChain;}

  inline const char * typeOf() const {return this->typeChain->data;}
  inline bool matchType(TypedClass& t) const {return t.typeOf()==this->typeOf();}
  inline bool matchType(const char * type) const {return type==this->typeOf();}

  inline bool isTypeOf(TypedClass& t) const {
    Chain<const char*> * c=this->typeChain;
    while(c){
      if(c->data==t.typeOf())
        return true;
      c=c->next();}
    return false;
  }

  inline bool isTypeOf(const char* type) const {
    Chain<const char*> * c=this->typeChain;
    while(c){
      if(c->data==type)
        return true;
      c=c->next();}
    return false;
  }
};


class GCRefLink {
  friend class GCRef;
private:
  GCRef * ref;
  GCRefLink(GCRef * ref);

public:
  ~GCRefLink();
};

class GCRef {
  friend class GCRefLink;
private:
  int refCount;
  bool lock;

  inline void unlink();

public:
  GCRefLink * link();
  void unlock();
  int refUsersCount() const;

protected:
  GCRef();
  virtual ~GCRef(){}
};

#endif // GC_H
