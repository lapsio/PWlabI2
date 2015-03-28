#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object : public PhysicalBody, public LightSource, public Decal
{
private:
  static int maxId;
  const std::string name;
  int id;
public:
  Object(const std::string name=std::string("Object"),
         const LightSource&=LightSource(),
         const PhysicalBody&=PhysicalBody());
  Object(const Object& ref);
  virtual ~Object();

  virtual int typeOf();
};

class Decal
{
private:
public:
  Decal();
  virtual ~Decal(){};
};

class LightSource
{
private:
public:
  LightSource();
  virtual ~LightSource(){};
};

class PhysicalBody
{
private:
  XyPair * boundBox;
  struct Array
      *collisionMesh,
      *collisionNormals;

  enum ObjectType {passive='p', dynamic='d'} type;
  enum CollisionType {ghost='g',solid='s'} collisionType;
  enum MeshType {circle='c', mesh='m'} meshType;

  double friction, mass;

public:
  PhysicalBody();
  virtual ~PhysicalBody();

  void reshape(struct Array mesh);
};


class PhysicsEngine
{
private:
  GameMap* map;

  void moveObjects(int objIndex=-1);
  void moveAnchors();

  void collideObjects(int objIndex=-1);

  void postMotion();
  inline void postMotion(int objIndex);

};

#endif // OBJECT_H
