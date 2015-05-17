#ifndef GFX_H
#define GFX_H

#include "include/object.h"
#include "SFML/Graphics.hpp"

class RenderEngine {
public:
  static constexpr int defaultBufferSize=256;
  static constexpr char* defaultWindowName=(char*)"Game window";

  struct objRecord {
    const PointXY* pos;
    const Decal* dec;
  };

private:
  sf::RenderWindow& window;
  PointXY canvasSize;
  PointXY cameraPos;

  objRecord* objList;
  int objListLength;
  int objListBufferSize;

  const bool autoresize;
  const bool resizable;

public:
  RenderEngine(int objBufferSize=RenderEngine::defaultBufferSize,
               bool autoresize=false,
               bool resizeable=true,
               const char* title=RenderEngine::defaultWindowName);

  RenderEngine(bool autoresize,
               bool resizeable=true,
               const char* title=RenderEngine::defaultWindowName);

  ~RenderEngine();

  void pushObject(const PointXY& position, const Decal& decal);
  void clear();
  void flush();

  void centerCamera(const PointXY pos);
  void canvasResize(int W, int H);
  void resizeDrawBuffer(int maxObjCount);
  void setWindowTitle(const char* title);
};

#endif // GFX_H
