#define _DEBUG

#ifdef _DEBUG
#include "./include/tests.h"
#endif

#include <iostream>
#include "./include/physx.h"

using namespace std;



int main()
{
#ifdef _DEBUG
  CORE_DEBUG();
#endif

  return 0;
}
