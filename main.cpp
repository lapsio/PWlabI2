#include "./include/defines.h"

#ifdef _DEBUG
#include "./include/tests.h"
#endif

#include <iostream>
#include "./include/physx.h"

using namespace std;



int main()
{
#ifdef _DEBUG

  test_punkt();
  test_wektor();
  CORE_DEBUG();
#endif

  return 0;
}
