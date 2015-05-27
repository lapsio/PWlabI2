
#include "include/gfx.h"

RenderEngine::RenderEngine(int objBufferSize, bool autoresize, bool resizable, const char* title) :
  window(*(new sf::RenderWindow(sf::VideoMode(800,600),title))),
  canvasSize(PointXY(800,600)),
  cameraPos(PointXY(this->canvasSize.getX()/2,this->canvasSize.getY()/2)),
  objList(new objRecord[objBufferSize]),
  objListLength(0),
  objListBufferSize(objBufferSize),
  autoresize(autoresize),
  resizable(resizable)
{
  this->clear();
  this->flush();
}

RenderEngine::RenderEngine(bool autoresize, bool resizable, const char* title) :
  RenderEngine(RenderEngine::defaultBufferSize,autoresize,resizable,title)
{

}

RenderEngine::~RenderEngine(){
  delete [] this->objList;
  this->window.close();
}

void RenderEngine::pushObject(const PointXY pos, const Decal &decal){
  if (this->objListLength>=this->objListBufferSize){
    if (this->autoresize)
      this->resizeDrawBuffer(this->objListBufferSize*2);
    else
      throw "Objects list buffer too small";
  }

  this->objList[this->objListLength++]={pos,&decal};
}

void RenderEngine::clear(){
  this->objListLength=0;
}

void RenderEngine::flush(){
  window.clear();
  PointXY pivot=PointXY(this->window.getSize().x,this->window.getSize().y);
  pivot-=this->canvasSize;
  pivot*=0.5;

  pivot-=this->cameraPos;
  pivot+=this->canvasSize*0.5;

  for (int i = 0; i < this->objListLength ; i++){
    sf::Sprite sp(*(this->objList[i].dec->texture));
    sp.setPosition(
          this->objList[i].pos.getX()*RenderEngine::UNITSIZE+pivot.getX(),
          this->objList[i].pos.getY()*RenderEngine::UNITSIZE+pivot.getY());
    this->window.draw(sp);
  }
  window.display();
}

void RenderEngine::centerCamera(const PointXY position){
  this->cameraPos=position*RenderEngine::UNITSIZE;
}

void RenderEngine::canvasResize(const PointXY size){
  this->canvasSize=size*RenderEngine::UNITSIZE;
}

void RenderEngine::resizeDrawBuffer(int maxObjCount){
  if (!this->resizable)
    throw "Buffer not resizable";
  if (maxObjCount<this->objListLength)
    throw "Buffer too small, you can try to clear screen before shrinking buffer";

  objRecord* newBuffer=new objRecord[maxObjCount];
  for (int i = 0 ; i < this->objListLength ; i++)
    newBuffer[i]=this->objList[i];
  delete this->objList;
  this->objList=newBuffer;
}

void RenderEngine::setWindowTitle(const char *title){
  this->window.setTitle(title);
}
