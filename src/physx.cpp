#include "./include/physx.h"



ObjectPhysicsMeta::ObjectPhysicsMeta(const ObjectMapMeta& ref, const PhysicsEngine& engine, VectorXY speed, VectorXY acceleration) :
  ObjectMapMeta(ref),
  engine(&engine),
  gridNeighbours(nullptr),
  speed(speed),
  acceleration(acceleration)
{
  this->pushType(ObjectPhysicsMeta::typeName);
  this->gridNeighbours=new Chain<PhysicsEngine::CollisionGrid::GridPool*> (nullptr);
}

ObjectPhysicsMeta::~ObjectPhysicsMeta(){
  delete this->gridNeighbours;
  this->engine=nullptr;
}

void ObjectPhysicsMeta::linkMap(GameMap &map){
  if (this->inUse)
    throw "meta already in use";

  if (this->engine->getMap()!=&map)
    throw "cannot place physics meta on different map";

  this->inUse=true;
  this->map=&map;
  this->mapLink=this->link();
}

void ObjectPhysicsMeta::unlinkMap(){
  this->inUse=false;
  this->map=nullptr;
  delete this->mapLink;

  Chain<PhysicsEngine::CollisionGrid::GridPool*> * c = this->gridNeighbours;
  while (c){
    Chain<ObjectPhysicsMeta*> * d = c->data->objectsChain;
    while (d){
      if (d->data==this){
        delete d;
        break;
      }
      d=d->next();
    }
    if (!d)
      throw "Could not find meta on grid - internal engine error";
    c=c->next();
  }
  this->gridNeighbours=nullptr;
}

PhysicsEngine::CollisionGrid::CollisionGrid(GameMap& map) :
  map(&map),
  gridW(this->computeDim(&map).a),
  gridH(this->computeDim(&map).b),
  gridC(this->map->width/this->gridW),
  gridR(this->map->height/this->gridH)
{
  int R = this->gridR,
      C = this->gridC,
      tileW = this->gridW,
      tileH = this->gridH;

  struct PhysicsEngine::CollisionGrid::GridPool *** grid = (struct PhysicsEngine::CollisionGrid::GridPool***)malloc(sizeof(struct PhysicsEngine::CollisionGrid::GridPool **)*R);
  for (int r = 0 ; r < R ; r++){
    grid[r] = (struct PhysicsEngine::CollisionGrid::GridPool **)malloc(sizeof (struct PhysicsEngine::CollisionGrid::GridPool *)*C);
    for (int c = 0 ; c < C ; c++){
      grid[r][c]=(struct PhysicsEngine::CollisionGrid::GridPool*)malloc(sizeof(struct PhysicsEngine::CollisionGrid::GridPool));
      grid[r][c]->posX=c*tileW;
      grid[r][c]->posY=r*tileH;
      grid[r][c]->indR=r;
      grid[r][c]->indC=c;
      grid[r][c]->objectsChain=new Chain<ObjectPhysicsMeta*>(nullptr);
    }
  }
  this->grid=grid;
}

PhysicsEngine::CollisionGrid::~CollisionGrid(){
  for (int r = 0 ; r < this->gridR ; r++)
    for (int c = 0 ; c < this->gridC ; c++)
      if (this->grid[r][c]->objectsChain->next())
        throw "Cannot delete - grid still in use!";
      else {
        delete this->grid[r][c]->objectsChain;
        free(this->grid[r][c]);
      }
  free (this->grid);
}

PhysicsEngine::CollisionGrid::whp PhysicsEngine::CollisionGrid::computeDim(GameMap *map){
  int tileW=map->width,
      tileH=map->height;
  for (int i = map->width-1; i > 1 ; i--){
    if (tileW<map->width/2 && (map->width/i>15 || i<map->width/8))
      break;
    if (map->width%i==0)
      tileW=i;
  }
  for (int i = map->height-1; i > 1 ; i--){
    if (tileH<map->height/2 && (map->height/i>15 || i<map->height/8))
      break;
    if (map->height%i==0)
      tileH=i;
  }

  //try more rectangular

  if (tileH*2<tileW){
    for (int i = tileW ; i < tileH ; i--)
      if (map->width%i==0)
        tileW=i;
  } else if (tileH>tileW*2){
    for (int i = tileH ; i > tileW ; i--)
      if (map->height%i==0)
        tileH=i;
  }
/*
  int C=map->width/tileW,
      R=map->height/tileH;

  this->gridC=C;
  this->gridR=R;
  this->gridW=tileW;
  this->gridH=tileH;
  */
  return {tileW,tileH};
}

VectorXY PhysicsEngine::CollisionGrid::getObjectBounds(ObjectPhysicsMeta &m){
  VectorXY bound = VectorXY({0,0},m.object.boundBox);

  PointXY
      lt = PointXY(0,0),
      rb = PointXY(0,0);

  lt.X=bound.getBegin().X;
  if (m.speed.width()<0)
    lt.X+=m.speed.width();

  lt.Y=bound.getBegin().Y;
  if (m.speed.height()<0)
    lt.Y+=m.speed.height();

  rb.X=bound.getEnd().X;
  if (m.speed.width()>0)
    rb.X+=m.speed.width();

  rb.Y=bound.getEnd().Y;
  if (m.speed.height()>0)
    rb.Y+=m.speed.height();

  if (m.object.meshType==PhysicalBody::MeshType::circle){
    lt.X-=bound.width();
    lt.Y-=bound.height();
  }

  bound = VectorXY(lt,rb);
  bound.moveBy(m.pos);

  return bound;
}

void PhysicsEngine::CollisionGrid::eraseMetaRecords(ObjectPhysicsMeta *meta){
  //delete old entries

  Chain<PhysicsEngine::CollisionGrid::GridPool*> * gridChain = meta->gridNeighbours->next();
  while (gridChain){
    struct PhysicsEngine::CollisionGrid::GridPool * currPool = gridChain->data;
    Chain<ObjectPhysicsMeta*>*metaChain = currPool->objectsChain->next();
    while (metaChain){
      if (metaChain->data==meta){
        metaChain=metaChain->prev();
        delete metaChain->next();
      }
      metaChain=metaChain->next();
    }
    gridChain=gridChain->next();
  }

  while (meta->gridNeighbours->next())
    delete meta->gridNeighbours->next();//leave only NULL entry
}

void PhysicsEngine::CollisionGrid::registerMeta(ObjectPhysicsMeta &meta){
  this->eraseMetaRecords(&meta);

  VectorXY objBounds = PhysicsEngine::CollisionGrid::getObjectBounds(meta);

  int minC = 0,
      maxC = this->gridC-1,
      minR = 0,
      maxR = this->gridR-1;

  while ((minC+1)*this->gridW<objBounds.getBegin().X)
    minC++;
  while ((maxC)*this->gridW>objBounds.getEnd().X)
    maxC--;

  while ((minR+1)*this->gridH<objBounds.getBegin().Y)
    minR++;
  while ((maxR)*this->gridH>objBounds.getEnd().Y)
    maxR--;

  for (int r = minR ; r < maxR+1 ; r++)
    for (int c = minC ; c < maxC+1 ; c++){
      meta.gridNeighbours->insertAfter(this->grid[r][c]);//chainInsertElemAfter(obj->gridChain,map->collisionGrid[r][c]);
      this->grid[r][c]->objectsChain->insertAfter(&meta);//chainInsertElemAfter(map->collisionGrid[r][c]->objects,obj);
    }
}

PhysicsEngine::PhysicsEngine(GameMap &map) :
  map(&map)
{

}
