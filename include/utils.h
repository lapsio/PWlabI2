#ifndef GC_H
#define GC_H

#include "./include/defines.h"
#include "./include/misc.h"
#include "string.h"

class TypedClass;
class GCRefLink;
class GCRef;

/**
 * @brief The TypedClass class class meant to be inherited by classes meant to be downcasted, providing basic "instanceof" functionality.
 */
class TypedClass
{
private:
  Chain<const char*>* typeChain;

protected:
  TypedClass();

  /**
   * @brief pushType Adds new type descriptor to registered inheritance tree
   * @param type new class type descriptor
   */
  inline void pushType(const char * type){this->typeChain->insertBefore(type);this->typeChain=this->typeChain->prev();}

public:
  static constexpr char * typeName=(char*)"Generic";  /**< type descriptor*/

  virtual ~TypedClass(){while(this->typeChain->next())delete this->typeChain->next();delete this->typeChain;}

  /**
   * @brief typeOf returns highest level type name
   * @return type name
   */
  inline const char * typeOf() const {return this->typeChain->data;}

  /**
   * @brief matchType tells whether class is type as provided reference class
   * @param t class to be compared with
   * @return yup or nope
   */
  inline bool matchType(TypedClass& t) const {return !strcmp(t.typeOf(),this->typeOf());}

  /**
   * @brief matchType tells whether class is type as provided type descriptor
   * @param type descriptor to be compared with
   * @return yup or nope
   */
  inline bool matchType(const char * type) const {return !strcmp(type,this->typeOf());}

  /**
   * @brief isTypeOf tells whether current class inherits from specified class
   * @param t reference class
   * @return yup or nope
   */
  inline bool isTypeOf(TypedClass& t) const {
    Chain<const char*> * c=this->typeChain;
    while(c){
      if(!strcmp(c->data,t.typeOf()))
        return true;
      c=c->next();}
    return false;
  }

  /**
   * @brief isTypeOf tells whether current class inherits from specified class descriptor
   * @param type class descriptor
   * @return yup or nope
   */
  inline bool isTypeOf(const char* type) const {
    Chain<const char*> * c=this->typeChain;
    while(c){
      if(!strcmp(c->data,type))
        return true;
      c=c->next();}
    return false;
  }
};


/**
 * @brief The GCRefLink class Helper class to GCRefLink, once deleted - GCRef class will unbind reference user and if it was last reference - delete data
 * @see GCRef
 */
class GCRefLink {
  friend class GCRef;
private:
  GCRef * ref;
  GCRefLink(GCRef * ref);

public:
  ~GCRefLink();
};

/**
 * @brief The GCRef class Class providing basic garbage collecting functionality similar to one provided by sharedptr.
 * User can get link which can be deleted later to inform that certain code won't use data anymore. Once all links get deleted class is deleting itself on its own.
 * New class has 0 links thus it's locked. It can be manually unlocked, then deleted or it unlocks automatically after registering first link.
 */
class GCRef {
  friend class GCRefLink;
private:
  int refCount;
  bool lock;

  inline void unlink();

public:

  /**
   * @brief link Get new link to indicate data usage
   * @return link
   */
  GCRefLink * link();

  /**
   * @brief unlock Manually unlocks object
   */
  void unlock();

  /**
   * @brief refUsersCount Returns current number of registered links
   * @return number of current users
   */
  int refUsersCount() const;

protected:
  GCRef();
  virtual ~GCRef(){}
};

#endif // GC_H
