#include <stdlib.h>

void operator delete(void* object) {
    free(object);
}

void* operator new(size_t objectSize) {
    return malloc(objectSize);
}
