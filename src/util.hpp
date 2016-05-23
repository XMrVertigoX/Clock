#ifndef UTIL_HPP_
#define UTIL_HPP_

void* operator new(size_t object);
void operator delete(void* object);
void* operator new[](size_t object);
void operator delete[](void* object);

#endif /* UTIL_HPP_ */
