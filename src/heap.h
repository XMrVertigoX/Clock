#ifndef HEAP_H_
#define HEAP_H_

#include <stdlib.h>

void operator delete(void* obj);
void* operator new(size_t objsize);

#endif
