#include <cstdio>

#include "class.h"

A::A() { printf("A::A()\n"); }
int A::static_int = 101;
const int A::const_static_int;
