#include "./include/defines.h"

#ifdef _DEBUG
#include "./include/tests.h"
#endif

#include <iostream>
#include "./include/engine.h"

using namespace std;



int main()
{
#ifdef _DEBUG
  test_punkt();
  test_wektor();
  CORE_DEBUG();
#endif

  bool interrupt=false;

  GameEngine * game = new GameEngine();
  game->start(interrupt);

  return 0;
}
