#ifndef GFX_H
#define GFX_H

#include "include/object.h"
#include "SFML/Graphics.hpp"

/**
 * @brief The RenderEngine class rendering engine for game
 */

class RenderEngine {
public:
  static constexpr int UNITSIZE=10; /**< size of single session-wise unit in px*/

  static constexpr int defaultBufferSize=256; /**< default draw buffer size*/
  static constexpr char* defaultWindowName=(char*)"Game window";

  struct objRecord {
    PointXY pos;
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
  /**
   * @brief RenderEngine basic constructor
   * @param objBufferSize initial size of draw buffer (max number of rendered Decals)
   * @param autoresize whether buffer should auto resize
   * @param resizeable whether buffer should allow manual resizing
   * @param title window title
   */
  RenderEngine(int objBufferSize=RenderEngine::defaultBufferSize,
               bool autoresize=false,
               bool resizeable=true,
               const char* title=RenderEngine::defaultWindowName);

  RenderEngine(bool autoresize,
               bool resizeable=true,
               const char* title=RenderEngine::defaultWindowName);

  ~RenderEngine();

  /**
   * @brief pushObject add new Decal to render queue
   * @param position position of decal to be rendered (in session-wise units)
   * @param decal decal to be rendered
   */
  void pushObject(const PointXY position, const Decal& decal);
  /**
   * @brief clear clear draw buffer
   */
  void clear();
  /**
   * @brief flush flush draw buffer
   */
  void flush();

  /**
   * @brief centerCamera center camera on specified point
   * @param pos new camera center point (provided in session-wise units)
   */
  void centerCamera(const PointXY pos);
  inline void centerCamera(int W, int H){this->centerCamera(PointXY(W,H));}
  /**
   * @brief canvasResize resize virtual canvas
   * @param pos new size of canvas
   */
  void canvasResize(const PointXY size);
  inline void canvasResize(int W, int H){this->canvasResize(PointXY(W,H));}
  /**
   * @brief resizeDrawBuffer resize draw buffer
   * @param maxObjCount new max objects count
   */
  void resizeDrawBuffer(int maxObjCount);
  /**
   * @brief setWindowTitle set window title
   * @param title
   */
  void setWindowTitle(const char* title);
};

#endif // GFX_H
