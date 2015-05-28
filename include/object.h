#ifndef OBJECT_H
#define OBJECT_H

#include "./include/defines.h"

#include <string>
#include <cstring>
#include "./include/PointXY.h"
#include "./include/VectorXY.h"
#include "./include/misc.h"
#include "./include/utils.h"
#include "SFML/Graphics.hpp"



class Object;
class Decal;
class LightSource;
class PhysicalBody;



class Decal
{
private:
  const sf::Texture* loadTexture(const char* path);

public:
  static constexpr char* voidTexturePath=(char*)"misc/void.png";

  const sf::Texture* texture;
  const PointXY offset;

  Decal(const char* filePath=Decal::voidTexturePath, PointXY offset=PointXY(0,0));
  virtual ~Decal();
};

class LightSource
{
private:
public:
  LightSource(){}
  virtual ~LightSource(){}
};


/**
 * @brief The PhysicalBody class Class providing basic physical capabilities.
 */
class PhysicalBody
{
  friend class PhysicsEngine;

public:
  enum class ObjectType {passive='p', dynamic='d'}; /**< type of object - passive means it's not moving */
  enum class CollisionType {ghost='g',solid='s'}; /**< collision type determines whether objects will be colliding with this object or not */
  enum class MeshType {circle='c', mesh='m'}; /**< mesh type - circular object or polygon with collision mesh */

private:
  PointXY boundBox;
  Array<VectorXY*> *collisionMesh, *collisionNormals;

  ObjectType type;
  CollisionType collisionType;
  MeshType meshType;

  double friction, mass;

  void computeNormals();
  void meshRealloc(Array<VectorXY*>& col, Array<VectorXY*>& nor);

public:

  /**
   * @brief PhysicalBody Constructor without mesh (circle by default)
   * @param type object type
   * @param collision collision type
   * @param dimensions object dimensions as w:h pair
   * @param friction object friction factor
   * @param mass object mass
   */
  PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, double friction=1, double mass=1);
  /**
   * @brief PhysicalBody Constructor with mesh
   * @param type object type
   * @param collision collision type
   * @param dimensions object dimensions as w:h pair
   * @param collisionMesh polygon representing collision mesh provided as set of vertices
   * @param friction object friction factor
   * @param mass object mass
   */
  PhysicalBody(ObjectType type, CollisionType collision, PointXY dimensions, Array<PointXY*>& collisionMesh, double friction=1, double mass=1);
  /**
   * @brief PhysicalBody copy constructor
   * @param ref reference object
   */
  PhysicalBody(const PhysicalBody& ref);
  virtual ~PhysicalBody();

  /**
   * @brief getBound returns object bouding box (dimensions)
   * @return bouding box w:h pair
   */
  const PointXY& getBound() const {return this->boundBox;}
  /**
   * @brief getCollisionMesh
   * @return object collision mesh
   */
  const Array<VectorXY*>& getCollisionMesh() const {return *((this)->collisionMesh);}
  /**
   * @brief getCollisionNormals
   * @return object normals
   */
  const Array<VectorXY*>& getCollisionNormals() const {return *((this)->collisionNormals);}

  /**
   * @brief getObjectType
   * @return object type
   */
  inline ObjectType getObjectType() const {return this->type;}
  /**
   * @brief getCollisionType
   * @return object collision type
   */
  inline CollisionType getCollisionType() const {return this->collisionType;}
  /**
   * @brief getMeshType
   * @return object mesh type
   */
  inline MeshType getMeshType() const {return this->meshType;}

  /**
   * @brief getFriction
   * @return object friction factor
   */
  inline double getFriction() const {return this->friction;}
  /**
   * @brief getMass
   * @return object mass
   */
  inline double getMass() const {return this->mass;}

  /**
   * @brief reshape strips object collision mesh, turning object into circle type
   */
  void reshape();
  /**
   * @brief reshape assigns collision mesh polygon to object turning it into mesh (polygon) object type
   * @param collisionMesh
   */
  void reshape(Array<PointXY*>& collisionMesh);

  /**
   * @brief setObjectType
   * @param type new object type
   */
  inline void setObjectType(ObjectType type){this->type=type;}
  /**
   * @brief setCollisionType
   * @param collision new collision type
   */
  inline void setCollisionType(CollisionType collision){this->collisionType=collision;}
  /**
   * @brief setMass
   * @param mass new mass (can't be negative)
   */
  inline void setMass(double mass){mass>0?this->mass=mass:throw "mass must be positive value";}
  /**
   * @brief setFriction
   * @param friction friction factor (can't be negative)
   */
  inline void setFriction(double friction){friction>0?this->friction=friction:throw "friction must be positive value";}

  /**
   * @brief operator = assignment operator
   * @param R
   * @return
   */
  PhysicalBody& operator=(const PhysicalBody& R);

#ifdef _DEBUG
  static void DEBUG(PhysicalBody * o){
    std::cout << "bounds: " << o->boundBox.getX() << " " << o->boundBox.getY() << std::endl;
    std::cout << "flags: " << (char)o->type << " " << (char)o->collisionType << " " << (char)o->meshType << std::endl;
    std::cout << "props: mass: " << o->mass << " friction: " << o->friction << std::endl;
    if (o->meshType==PhysicalBody::MeshType::mesh){
      std::cout << "mesh: " << std::endl;

      int l = o->collisionMesh->length();
      for (int i = 0 ; i < l ; i++)
        std::cout
            << "   " << o->collisionMesh->getData(i)->getBegin().getX()
            << ":" << o->collisionMesh->getData(i)->getBegin().getY()
            << "   " << o->collisionMesh->getData(i)->getEnd().getX()
            << ":" << o->collisionMesh->getData(i)->getEnd().getY() << std::endl;

      std::cout << "normals: " << std::endl;

      l = o->collisionNormals->length();
      for (int i = 0 ; i < l ; i++)
        std::cout
            << "   " << o->collisionNormals->getData(i)->getBegin().getX()
            << ":" << o->collisionNormals->getData(i)->getBegin().getY()
            << "   " << o->collisionNormals->getData(i)->getEnd().getX()
            << ":" << o->collisionNormals->getData(i)->getEnd().getY() << std::endl;
    } else
      std::cout << "no mesh - circular object" << std::endl << std::endl;
  }
#endif
};

/**
 * @brief The Object class Basic object class
 */
class Object : public PhysicalBody, public LightSource, public Decal, public GCRef, public TypedClass
{
public:
  static constexpr char * typeName=(char*)"Object"; /**< object type name */

private:
  const std::string name;
  const long long id;

  static long long maxId;
public:
  static const Object nullObj; /**< dummy object for placeholder purpose */

  /**
   * @brief Object Object constructor
   * @param name name of new Object. Can be used by game domain code
   */
  Object(const std::string name = std::string("Object"),
         const Decal& = Decal(),
         const PhysicalBody& = PhysicalBody(PhysicalBody::ObjectType::passive, PhysicalBody::CollisionType::ghost, PointXY(0,0)),
         const LightSource& = LightSource());
  /**
   * @brief Object Copy constructor
   * @param ref reference object
   */
  Object(const Object& ref);
  virtual ~Object();

  /**
   * @brief getName
   * @return name of object
   */
  inline const std::string getName() const {return this->name;}
  /**
   * @brief operator == compare operator
   * @param o
   * @return
   */
  inline bool operator==(const Object& o) const {return o.id==this->id;}
  /**
   * @brief operator != compare operator
   * @param o
   * @return
   */
  inline bool operator!=(const Object& o) const {return o.id!=this->id;}

#ifdef _DEBUG
  static void DEBUG(Object * o){
    std::cout << "type: " << o->typeOf() << std::endl;

    std::cout << "SELF: " << o << std::endl;

    std::cout << "name: " << o->name << std::endl;
    std::cout << "id:" << o->id << std::endl;
    std::cout << "maxId:" << Object::maxId << std::endl;

    std::cout << "refUsers:" << o->refUsersCount() << std::endl;
  }
#endif
};


#endif // OBJECT_H
