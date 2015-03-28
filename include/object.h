#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object : public PhysicalBody, public LightSource, public Decal
{
private:
  std::string name;
public:
  Object(const char* name);
};

class Decal
{
private:
public:
  Decal();
};

class LightSource
{
private:
public:
  LightSource();
};

class PhysicalBody
{
private:
  XyPair * boundBox = nullptr;
  struct Array
      *collisionMesh = nullptr,
      *collisionNormals = nullptr;

  enum ObjectType {passive='p', dynamic='d'} type;
  enum CollisionType {ghost='g',solid='s'} collisionType;
  enum MeshType {circle='c', mesh='m'} meshType;

  double friction, mass;

public:
  PhysicalBody();
  XyVector * isColliding(PhysicalBody B);
  void reshape(struct Array mesh);
};

#endif // OBJECT_H
