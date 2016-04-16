#include <stdlib.h>

void operator delete(void* object) {
    free(object);
}

void* operator new(size_t object) {
    return malloc(object);
}
