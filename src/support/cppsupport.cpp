#include <stdlib.h>

void* __dso_handle = NULL;

extern "C" int __cxa_atexit(void* object, void (*destructor)(void*),
                            void* dso_handle) {
    static_cast<void>(object);
    static_cast<void>(destructor);
    static_cast<void>(dso_handle);

    return 0;
}
