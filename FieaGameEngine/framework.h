#pragma once
#include <new>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define GLM_ENABLE_EXPERIMENTAL         // Include experimental code from glm package

#ifdef _DEBUG
//#define malloc(_size) malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__);
#define NEW new (__FILE__, __LINE__)
inline void* operator new(size_t size, const char* file, int line) { 
	void* addr = _malloc_dbg(size, _NORMAL_BLOCK, file, line);
	return operator new(size, addr);
}
inline void operator delete(void* ptr, const char* file, int line) {
	return operator delete(ptr);
}
#else
#define NEW new
#endif
