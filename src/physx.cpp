#include <unistd.h>
#include "./include/physx.h"


Timer::Timer(int frequency, bool autosleep) :
  paused(true),
  autosleep(autosleep),
  freq(frequency)
{

}

bool Timer::shift(long double diff){
  if (this->paused)
    throw "Cannot shift paused timer";

  //std::cout << "CURR LAST TIME: " << this->lastTime << std::endl;

  this->bufferedTimeShift+=diff;

  unsigned long long t;
  struct timespec currentTime;
  clock_gettime(CLOCK_REALTIME,&currentTime);

  t=(unsigned long long)currentTime.tv_sec*1000;
  t+=currentTime.tv_nsec/1000000;

  //std::cout << t << " " << this->lastTime << " " << this->bufferedTimeShift << std::endl;

  if (t-this->lastTime>this->bufferedTimeShift)
    return false;

  if (this->autosleep&&t-this->lastTime<(1000/this->freq)){
    //std::cout << "WAIT" << (1000/this->freq) << " " << (t-this->lastTime) << std::endl;
    usleep(((1000/this->freq)-(t-this->lastTime))*1000);
  }

  this->bufferedTimeShift-=(t-this->lastTime);
  this->lastTime=t;

  //std::cout << "NEXT LAST TIME: " << this->lastTime << std::endl;

  return true;
}

void Timer::pause(){
  this->paused=true;
}

void Timer::resume(){
  this->paused=false;

  //std::cout << "TIME RESET" << std::endl;

  this->bufferedTimeShift=0;

  struct timespec currentTime;
  clock_gettime(CLOCK_REALTIME,&currentTime);

  this->lastTime=(unsigned long long)currentTime.tv_sec*1000;
  this->lastTime+=currentTime.tv_nsec/1000000;
}


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
  Chain<PhysicsEngine::CollisionGrid::GridPool*> * c = this->gridNeighbours;
  while ((c=c->next())){
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
  }
  this->gridNeighbours=nullptr;

  this->inUse=false;
  this->map=nullptr;
  delete this->mapLink;
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

Chain<PhysicsEngine::CollisionGrid::GridPool*>* PhysicsEngine::CollisionGrid::getNeighbours(ObjectPhysicsMeta &m){
  return m.gridNeighbours;
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
  collisionsRegistry(new Chain<PhysicsEngine::CREnt>({nullptr,nullptr}))
{
  this->loadMapData(map);
}

PhysicsEngine::~PhysicsEngine(){
  delete this->collisionGrid;
  Chain<PhysicsEngine::CREnt>*c=this->collisionsRegistry;
  while((c=this->collisionsRegistry->next()))
    delete c;
  delete this->collisionsRegistry;
}

void PhysicsEngine::loadMapData(GameMap &map){
  this->map=&map;
  this->collisionGrid=new PhysicsEngine::CollisionGrid(map);
}

void PhysicsEngine::purgeMapData(){
  int l = this->map->length();
  ObjectMapMeta * oldMeta;
  for(int i = 1; i < l ; i++){
    if ((oldMeta=&(this->map->getMeta(i)))->isTypeOf(ObjectPhysicsMeta::typeName)){
      ObjectMapMeta * newMeta = new ObjectMapMeta(*oldMeta);
      this->map->deleteObject(i);
      this->map->addObject(*newMeta);
    }
  }
  this->map=nullptr;
  delete this->collisionGrid;
}



long double PhysicsEngine::getTimeShift(int objIndex){
  //calculate smallest object and highest speed

  //fprintf(stderr,"timeShift begin (%Ldms)\n",timeDiff);
  long double
      min_bound=this->map->width+this->map->height,
      max_spd=0,
      tmp,
      timeStep;

  ObjectMapMeta * meta;
  ObjectPhysicsMeta * pmeta;
  Object * obj;
  for (int i = (objIndex==-1)?1:objIndex, l = (objIndex==-1)?this->map->length():objIndex+1; i < l ; i++){
    if ((meta=&(this->map->getMeta(i)))->isTypeOf(ObjectPhysicsMeta::typeName)&&(obj=&meta->object)->collisionType!=PhysicalBody::CollisionType::ghost){
      if (min_bound>(tmp=fabs(obj->boundBox.X)))
        min_bound=tmp;
      if (min_bound>(tmp=fabs(obj->boundBox.Y)))
        min_bound=tmp;

      pmeta = dynamic_cast<ObjectPhysicsMeta*>(meta);

      if (max_spd<(tmp=fabs(pmeta->speed.width())+fabs(pmeta->speed.height())))
        max_spd=tmp;
    }
  }

  min_bound/=4;

  //if(max_spd>0)
  //std::cout << "AAAAAAAAAAA " << max_spd << std::endl;

  if (max_spd==0)
    timeStep=0.25; //4fps on idle
  else
    timeStep=min_bound/max_spd;

  if (timeStep>0.25)
    timeStep=0.25;

  return timeStep;
  /*struct Chain * objChain = session->map->objectsChain;
  while (objChain){
    struct Object * obj = chainGetData(objChain);

    if (obj->collision!=ghost){
      if (min_bound>(tmp=fabs(xyVectorWidth(obj->bound))))
        min_bound=tmp;
      if (min_bound>(tmp=fabs(xyVectorHeight(obj->bound))))
        min_bound=tmp;

      if (max_spd<(tmp=fabs(xyVectorHeight(obj->speed))+fabs(xyVectorWidth(obj->speed))))
        max_spd=tmp;
    }

    objChain=chainGetNext(objChain);
  }
  min_bound/=2;

  if (max_spd==0)
    timeStep=0.25; //4fps on idle
  else
    timeStep=min_bound/max_spd;

  return timeStep;*/
}

void PhysicsEngine::moveObjects(long double &timeShift, int objIndex){
  //perform actual movement

  ObjectMapMeta * meta;
  ObjectPhysicsMeta * pmeta;
  Object * obj;
  for (int i = (objIndex==-1)?1:objIndex, l = (objIndex==-1)?this->map->length():objIndex+1; i < l ; i++){
    if (!(meta=&(this->map->getMeta(i)))->isTypeOf(ObjectPhysicsMeta::typeName)||
        ((pmeta=dynamic_cast<ObjectPhysicsMeta*>(meta))->getAnchor() ||
        (pmeta->speed.size()==0 && pmeta->acceleration.size()==0)))
      continue;


    meta->pos.show();

    //std::cout << pmeta->speed.width() << " " << meta->pos.X+pmeta->speed.height() << " " << timeShift;

    meta->pos.X+=pmeta->speed.width()*timeShift;
    meta->pos.Y+=pmeta->speed.height()*timeShift;

    meta->pos.show();

    //std::cout << pmeta->speed.width() << " " << meta->pos.X+pmeta->speed.height();

    //invalid pos correction

    obj=&meta->object;

    if (meta->pos.X<0)
      meta->pos.changeTo(0,meta->pos.Y);
    if (meta->pos.X+obj->boundBox.X>this->map->width)
      meta->pos.changeTo(this->map->width-obj->boundBox.X,meta->pos.Y);

    if (meta->pos.Y<0)
      meta->pos.changeTo(meta->pos.X,0);
    if (meta->pos.Y+obj->boundBox.Y>this->map->height)
      meta->pos.changeTo(meta->pos.X,this->map->height-obj->boundBox.Y);

    this->collisionGrid->registerMeta(*pmeta);
  }

  /*objChain = session->map->objectsChain;
  while (objChain){
    struct Object * obj = chainGetData(objChain);
    if (obj->anchor ||
        (xyVectorWidth(obj->speed) == xyVectorHeight(obj->speed) &&
        xyVectorWidth(obj->speed) == 0 &&
        xyVectorWidth(obj->acceleration) == xyVectorHeight(obj->acceleration) &&
        xyVectorWidth(obj->acceleration) == 0)){
      objChain=chainGetNext(objChain);
      continue;
    }

    xyVectorMove(obj->bound,
                 xyVectorWidth(obj->speed)*timeStep,
                 xyVectorHeight(obj->speed)*timeStep);

    //invalid pos correction

    if (obj->bound->beg->x<0)
      xyVectorMove(obj->bound,-obj->bound->beg->x,0);
    if (obj->bound->end->x>session->map->width)
      xyVectorMove(obj->bound,session->map->width-obj->bound->end->x,0);
    if (obj->bound->beg->y<0)
      xyVectorMove(obj->bound,0,-obj->bound->beg->y);
    if (obj->bound->end->y>session->map->height)
      xyVectorMove(obj->bound,0,session->map->height-obj->bound->end->y);

    collisionGridRegisterObject(session->map,obj);

    objChain=chainGetNext(objChain);
  }*/
}

void PhysicsEngine::collideObjects(long double &timeShift, int objIndex){
  (void)timeShift;
  //calculate collisions

  Chain<PhysicsEngine::CREnt> * c = this->collisionsRegistry;//clear collisions registry
  while (c->next())
    delete c->next();

  ObjectMapMeta * meta;
  ObjectPhysicsMeta * pmeta;
  //Object * obj;
  ObjectPhysicsMeta * collider;

  for (int i = (objIndex==-1)?1:objIndex, l = (objIndex==-1)?this->map->length():objIndex+1; i < l ; i++){
    if (!(meta=&(this->map->getMeta(i)))->isTypeOf(ObjectPhysicsMeta::typeName)||
        (pmeta=dynamic_cast<ObjectPhysicsMeta*>(meta))->getAnchor()||
        pmeta->speed.size()==0)
      continue;

    if ((collider=this->getObjectCollisions(*pmeta)))
      this->collisionsRegistry->insertAfter({pmeta,collider});
  }

  /*while (chainGetNext(session->world.lastCollisions))//clear last collision registry
    free(chainDeleteChainElem(chainGetNext(session->world.lastCollisions)));

  objChain = session->map->objectsChain;
  while (objChain){
    struct Object
        * obj = chainGetData(objChain),
        * collider;

    if (obj->anchor ||
        (xyVectorWidth(obj->speed) == xyVectorHeight(obj->speed) &&
        xyVectorWidth(obj->speed) == 0)){
      objChain=chainGetNext(objChain);
      continue;
    }

    if ((collider=objectGetCollision(obj,1))){
      struct Object ** t = malloc(sizeof(struct Object *)*2);
      t[0]=obj;
      t[1]=collider;
      chainInsertElemAfter(session->world.lastCollisions,t);
    }

    objChain=chainGetNext(objChain);
  }*/
}

ObjectPhysicsMeta * PhysicsEngine::getObjectCollisions(ObjectPhysicsMeta &meta, const bool& collide){
  Chain<PhysicsEngine::CollisionGrid::GridPool*> * c = this->collisionGrid->getNeighbours(meta);
  while((c=c->next())){
    Chain<ObjectPhysicsMeta*>*cc=c->data->objectsChain;
    while((cc=cc->next()))
      if (this->isColliding(meta,*cc->data,collide)>-1)
        return cc->data;
  }

  return nullptr;

  /*struct Chain * gridChain = chainGetNext(obj->gridChain);
  while(gridChain){
    struct Chain * objChain = chainGetNext(((struct GridPool*)chainGetData(gridChain))->objects);
    while (objChain){
      if (objectIsColliding(obj,chainGetData(objChain),collide)>-1)
        return chainGetData(objChain);
      objChain=chainGetNext(objChain);
    }
    gridChain = chainGetNext(gridChain);
  }

  return (struct Object*)NULL;*/
}

int PhysicsEngine::isColliding(ObjectPhysicsMeta &A, ObjectPhysicsMeta &B, const bool& collide){
  if (&A==&B ||
      (A.object.collisionType==PhysicalBody::CollisionType::ghost||B.object.collisionType==PhysicalBody::CollisionType::ghost) ||
      (A.object.type==PhysicalBody::ObjectType::passive && B.object.type==PhysicalBody::ObjectType::passive) ||
      (A.object.meshType==PhysicalBody::MeshType::mesh && B.object.meshType==PhysicalBody::MeshType::mesh))
    return -1;

  if (A.object.meshType==PhysicalBody::MeshType::circle && B.object.meshType==PhysicalBody::MeshType::circle){

  }

  if (A.object.meshType==PhysicalBody::MeshType::circle || B.object.meshType==PhysicalBody::MeshType::circle){
    //fprintf(stderr,"got candidates!: %s, %s\n",A->customID,B->customID);

    ObjectPhysicsMeta
        *o_circle=A.object.meshType==PhysicalBody::MeshType::circle?&A:&B,
        *o_mesh=A.object.meshType==PhysicalBody::MeshType::mesh?&A:&B;

    if (o_circle->getAnchor())
      return -1;

    PointXY localCirclePos = PointXY(
                               o_circle->pos.X-o_mesh->pos.X,
                               o_circle->pos.Y-o_mesh->pos.Y
                               );

    /*struct xyPoint * localCirclePos = xyPointCreate(
                               o_circle->bound->beg->x-o_mesh->bound->beg->x,
                               o_circle->bound->beg->y-o_mesh->bound->beg->y
                               );*/

    //fprintf(stderr,"local ball: %llf, %llf\n",
    //        localCirclePos->x,localCirclePos->y);

    //try edges

    for (int i = 0 ; i < o_mesh->object.collisionMesh->length() ; i++){

      //fprintf(stderr,"entering: %d",i);

      /*struct xyVector
          * face = arrayGetData(o_mesh->collisionMesh,i),
          * norm = xyVectorCenterOnPoint(
            xyVectorScaleTo(
              arrayGetData(o_mesh->collisionNormals,i),
              xyVectorWidth(o_circle->bound)*2
              ),
            localCirclePos
            );*/
      //fprintf(stderr,"face: (%llf;%llf),(%llf;%llf)\n",
      //        face->beg->x,face->beg->y,
      //        face->end->x,face->end->y);

      //fprintf(stderr,"norm: (%llf;%llf),(%llf;%llf)\n",
      //        norm->beg->x,norm->beg->y,
      //        norm->end->x,norm->end->y);

      VectorXY& vec = *o_mesh->object.getCollisionNormals()[i];
      vec.scaleTo(o_circle->object.boundBox.X*2);
      vec.centerOn(localCirclePos);
      if(VectorXY::intersects(vec,*o_mesh->object.getCollisionMesh()[i])){
        if (collide){
          VectorXY scaledSpeed = VectorXY(o_circle->speed);
          scaledSpeed.scaleTo(o_circle->object.boundBox.X*2);
          o_circle->pos.moveBy(
                scaledSpeed.width(),
                scaledSpeed.height());
          VectorXY::flipAcrossVector(
                o_circle->speed,
                *o_mesh->object.getCollisionNormals()[i]);
          o_circle->speed.flip();

          VectorXY projectedSpeed = VectorXY(o_mesh->speed);
          VectorXY::projectOntoVector(
                projectedSpeed,
                *o_mesh->object.getCollisionNormals()[i]);

          o_circle->speed.setEnd(
                o_circle->speed.getEnd().X+projectedSpeed.width(),
                o_circle->speed.getEnd().Y+projectedSpeed.height());
        }

        return i;
      }

      /*if (xyVectorIntersect(
            xyVectorCenterOnPoint(
              xyVectorScaleTo(
                arrayGetData(o_mesh->collisionNormals,i),
                xyVectorWidth(o_circle->bound)*2
                ),
              localCirclePos
              ),
            arrayGetData(o_mesh->collisionMesh,i)
            ))
      {
        if (collide){
          //fprintf(stderr,"COLLISION:\n");

          struct xyVector * scaledSpeed = xyVectorScaleTo(
                xyVectorCopy(o_circle->speed),
                xyVectorWidth(o_circle->bound)*2
                );

          //fprintf(stderr,"old ball: %llf, %llf\n",
          //        o_circle->bound->beg->x,o_circle->bound->beg->y);

          xyVectorMove(o_circle->bound,
                       -xyVectorWidth(scaledSpeed),
                       -xyVectorHeight(scaledSpeed));

          //fprintf(stderr,"new ball: %llf, %llf\n",
          //        o_circle->bound->beg->x,o_circle->bound->beg->y);

          xyVectorFlip(
                xyVectorFlipAgainstVector(
                  o_circle->speed,
                  arrayGetData(o_mesh->collisionNormals,i)
                  )
                );

          struct xyVector * projectedSpeed = xyVectorProjectOntoVector(
                xyVectorCopy(o_mesh->speed),
                arrayGetData(o_mesh->collisionNormals,i)
                );

          o_circle->speed->end->x+=xyVectorWidth(projectedSpeed);
          o_circle->speed->end->y+=xyVectorHeight(projectedSpeed);

          xyVectorReleaseFull(scaledSpeed);
          xyVectorReleaseFull(projectedSpeed);
        }

        xyPointRelease(localCirclePos);
        return i;
      }*/
    }

    //try verticles

    long double ballSize = o_circle->object.boundBox.X;

    for (int i = 0 ; i < o_mesh->object.getCollisionMesh().length() ; i++){
      VectorXY path = VectorXY(
                        o_mesh->object.getCollisionMesh()[i]->getBegin(),
                        localCirclePos);

      if (path.size()<ballSize){
        if (collide){
          VectorXY scaledSpeed = VectorXY(o_circle->speed);
          scaledSpeed.scaleTo(o_circle->object.boundBox.X*2);

          o_circle->pos.moveBy(
                -scaledSpeed.width(),
                -scaledSpeed.height());

          VectorXY::flipAcrossVector(o_circle->speed,path);
          o_circle->speed.flip();

          VectorXY projectedSpeed = VectorXY(o_mesh->speed);
          VectorXY::projectOntoVector(projectedSpeed,path);

          o_circle->speed.setEnd(
                o_circle->speed.getEnd().X+projectedSpeed.width(),
                o_circle->speed.getEnd().Y+projectedSpeed.height());
        }

        return i;
      }
    }

    /*long double ballSize = xyVectorWidth(o_circle->bound);

    for (int i = 0 ; i < arrayLength(o_mesh->collisionMesh) ; i++){
      struct xyVector * path = xyVectorCreate(
            ((struct xyVector*)arrayGetData(o_mesh->collisionMesh,i))->beg,
            localCirclePos);

      if (xyVectorLinearLength(path)<ballSize){
        if (collide){
          //fprintf(stderr,"VERT COLLISION:\n");

          struct xyVector * scaledSpeed = xyVectorScaleTo(
                xyVectorCopy(o_circle->speed),
                xyVectorWidth(o_circle->bound)*2);

          //fprintf(stderr,"old ball: %Lf, %Lf\n",
          //        o_circle->bound->beg->x,o_circle->bound->beg->y);

          xyVectorMove(o_circle->bound,
                       -xyVectorWidth(scaledSpeed),
                       -xyVectorHeight(scaledSpeed));

          //fprintf(stderr,"new ball: %Lf, %Lf\n",
          //        o_circle->bound->beg->x,o_circle->bound->beg->y);

          xyVectorFlip(
                xyVectorFlipAgainstVector(
                  o_circle->speed,
                  path));

          struct xyVector * projectedSpeed = xyVectorProjectOntoVector(
                xyVectorCopy(o_mesh->speed),
                path);

          o_circle->speed->end->x+=xyVectorWidth(projectedSpeed);
          o_circle->speed->end->y+=xyVectorHeight(projectedSpeed);

          xyVectorReleaseFull(scaledSpeed);
          xyVectorReleaseFull(projectedSpeed);
        }

        xyVectorReleasePartial(path);
        xyPointRelease(localCirclePos);
        return i;
      }
      xyVectorReleasePartial(path);
    }


    //fprintf(stderr,"not colliding\n");

    xyPointRelease(localCirclePos);
    return -1;
    */
    return -1;
  }
  return -1;
}

void PhysicsEngine::postMotion(long double &timeShift, int objIndex){
  //post motion effects

  ObjectMapMeta*meta;
  Object*obj;
  ObjectPhysicsMeta*pmeta;

  long double tmp;

  for (int i = (objIndex==-1)?1:objIndex, l = (objIndex==-1)?this->map->length():objIndex+1; i < l ; i++){
    if ((meta=&(this->map->getMeta(i)))->isTypeOf(ObjectPhysicsMeta::typeName)){

      pmeta=dynamic_cast<ObjectPhysicsMeta*>(meta);
      obj=&meta->object;

      pmeta->speed.setEnd(
            pmeta->speed.getEnd().X+pmeta->acceleration.width()*timeShift,
            pmeta->speed.getEnd().Y+pmeta->acceleration.height()*timeShift);

      if (obj->friction){
        if (pmeta->speed.width()>0)
          if (pmeta->speed.width()>(tmp=obj->friction*obj->mass*timeShift))
            pmeta->speed.setEnd(
                  pmeta->speed.getEnd().X-tmp,
                  pmeta->speed.getEnd().Y);
          else
            pmeta->speed.setEnd(
                  pmeta->speed.getBegin().X,
                  pmeta->speed.getEnd().Y);
        else
          if (pmeta->speed.width()<(tmp=-obj->friction*obj->mass*timeShift))
            pmeta->speed.setEnd(
                  pmeta->speed.getEnd().X-tmp,
                  pmeta->speed.getEnd().Y);
          else
            pmeta->speed.setEnd(
                  pmeta->speed.getBegin().X,
                  pmeta->speed.getEnd().Y);

        if (pmeta->speed.height()>0)
          if (pmeta->speed.height()>(tmp=obj->friction*obj->mass*timeShift))
            pmeta->speed.setEnd(
                  pmeta->speed.getEnd().X,
                  pmeta->speed.getEnd().Y-tmp);
          else
            pmeta->speed.setEnd(
                  pmeta->speed.getEnd().X,
                  pmeta->speed.getBegin().Y);
        else
          if (pmeta->speed.height()<(tmp=-obj->friction*obj->mass*timeShift))
            pmeta->speed.setEnd(
                  pmeta->speed.getEnd().X,
                  pmeta->speed.getEnd().Y-tmp);
          else
            pmeta->speed.setEnd(
                  pmeta->speed.getEnd().X,
                  pmeta->speed.getBegin().Y);
      }
    }
  }

  /*objChain = session->map->objectsChain;
  while (objChain){
    struct Object * obj = chainGetData(objChain);
    //acceleration

    obj->speed->end->x+=xyVectorWidth(obj->acceleration)*timeStep;
    obj->speed->end->y+=xyVectorHeight(obj->acceleration)*timeStep;

    //friction
    if (obj->friction){
      if(xyVectorWidth(obj->speed)>0)
        if (xyVectorWidth(obj->speed)>(tmp=obj->friction*obj->mass*timeStep))
          obj->speed->end->x-=tmp;
        else
          obj->speed->end->x=obj->speed->beg->x;
      else
        if (xyVectorWidth(obj->speed)<(tmp=-obj->friction*obj->mass*timeStep))
          obj->speed->end->x-=tmp;
        else
          obj->speed->end->x=obj->speed->beg->x;

      if(xyVectorHeight(obj->speed)>0)
        if (xyVectorHeight(obj->speed)>(tmp=obj->friction*obj->mass*timeStep))
          obj->speed->end->y-=tmp;
        else
          obj->speed->end->y=obj->speed->beg->y;
      else
        if (xyVectorHeight(obj->speed)<(tmp=-obj->friction*obj->mass*timeStep))
          obj->speed->end->y-=tmp;
        else
          obj->speed->end->y=obj->speed->beg->y;
    }

    objChain=chainGetNext(objChain);
  }*/
}

long double PhysicsEngine::timeShift(){
  long double time = this->getTimeShift();

  if (time == 0)
    throw "invalid time";

  this->moveObjects(time);
  this->collideObjects(time);
  this->postMotion(time);

  //std::cout << time << std::endl;

  return time*1000;
}

void PhysicsEngine::reloadMap(GameMap &map){
  (void)map;
  throw "Not implemented";
}

void PhysicsEngine::registerObject(ObjectMapMeta &meta){
  if(meta.isTypeOf(ObjectPhysicsMeta::typeName))
    throw "Meta already registered!";
  ObjectPhysicsMeta* newMeta = new ObjectPhysicsMeta(meta, *this);
  map->deleteObject((*(this->map))[meta]);
  map->addObject(*newMeta);
  this->collisionGrid->registerMeta(*newMeta);
}

void PhysicsEngine::unregisterObject(ObjectPhysicsMeta &meta){
  ObjectMapMeta* newMeta = new ObjectMapMeta(meta);
  map->deleteObject(&meta);
  map->addObject(*newMeta);
}

#ifdef _DEBUG
  void PhysicsEngine::DEBUG(PhysicsEngine * p){

    for (int i = 1, l = p->map->length(), k=1 ; i < l ; i++){
      ObjectMapMeta*m=&(*(p->map))[i];
      if (m->isTypeOf(ObjectPhysicsMeta::typeName)){
        std::cout << std::endl << "#" << k++ << ":" << std::endl;
        ObjectPhysicsMeta::DEBUG(dynamic_cast<ObjectPhysicsMeta*>(m));
        Object::DEBUG(&m->object);
      }
      else
        std::cout << std::endl << "#" << k++ << " - non-physical" << std::endl;
    }
  }
#endif
