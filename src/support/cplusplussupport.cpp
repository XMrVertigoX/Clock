#include "FreeRTOS.h"

void* operator new(size_t size) {
    return pvPortMalloc(size);
}

void* operator new[](size_t size) {
    return pvPortMalloc(size);
}

void operator delete(void* pointer) {
    vPortFree(pointer);
}

void operator delete(void* pointer, size_t size) {
    operator delete(pointer);
}

void operator delete[](void* pointer) {
    vPortFree(pointer);
}

void operator delete[](void* pointer, size_t size) {
    operator delete[](pointer);
}

void* __dso_handle = NULL;

extern "C" int __cxa_atexit(void* object, void (*destructor)(void*),
                            void* dso_handle) {
    static_cast<void>(object);
    static_cast<void>(destructor);
    static_cast<void>(dso_handle);
    return 0;
}
