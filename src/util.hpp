#ifndef HEAP_H_
#define HEAP_H_

#include <stdlib.h>

#define waitUntil(x) while (!x)

void operator delete(void* object);
void* operator new(size_t object);

#endif
