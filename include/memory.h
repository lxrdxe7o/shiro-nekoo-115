#ifndef HMS_MEMORY_H
#define HMS_MEMORY_H

#include <stddef.h>

// Tracked memory allocation functions
void* hms_malloc(size_t size, const char* file, int line);
void* hms_calloc(size_t count, size_t size, const char* file, int line);
void* hms_realloc(void* ptr, size_t size, const char* file, int line);
void hms_free(void* ptr);

// Memory statistics and debugging
void hms_memory_stats(size_t* allocated, size_t* freed, size_t* peak);
void hms_memory_leak_check(void);

// Convenience macros
#define MALLOC(size) hms_malloc(size, __FILE__, __LINE__)
#define CALLOC(count, size) hms_calloc(count, size, __FILE__, __LINE__)
#define REALLOC(ptr, size) hms_realloc(ptr, size, __FILE__, __LINE__)
#define FREE(ptr) hms_free(ptr)

#endif // HMS_MEMORY_H
