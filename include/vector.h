#ifndef HMS_VECTOR_H
#define HMS_VECTOR_H

#include <stddef.h>
#include "types.h"

typedef struct {
    void** items;           // Array of pointers to items
    size_t size;            // Number of items currently stored
    size_t capacity;        // Allocated capacity
    FreeFunc free_fn;       // Function to free items when removed/destroyed
} Vector;

// Creation and destruction
Vector* vector_create(size_t initial_capacity, FreeFunc free_fn);
void    vector_destroy(Vector* v);

// Access
void*   vector_get(Vector* v, size_t index);
void*   vector_last(Vector* v);
void*   vector_first(Vector* v);

// Modification
void    vector_push(Vector* v, void* item);
void*   vector_pop(Vector* v);
void    vector_set(Vector* v, size_t index, void* item);
void    vector_remove(Vector* v, size_t index);
void    vector_remove_fast(Vector* v, size_t index); // Swaps with last, generic O(1)
void    vector_clear(Vector* v);

// Search and Sort
void*   vector_find(Vector* v, CompareFunc cmp, const void* target);
int     vector_find_index(Vector* v, CompareFunc cmp, const void* target);
void    vector_sort(Vector* v, CompareFunc cmp);

// Utilities
size_t  vector_size(Vector* v);
bool    vector_empty(Vector* v);

#endif // HMS_VECTOR_H
