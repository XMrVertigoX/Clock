#include "FreeRTOS.h"

void* operator new(size_t object) {
    return pvPortMalloc(object);
}

void operator delete(void* object) {
    vPortFree(object);
}

void* operator new[](size_t object) {
    return operator new(object);
}

void operator delete[](void* object) {
    operator delete(object);
}
