#include "../include/vector.h"
#include "../include/memory.h"
#include <stdlib.h>
#include <string.h>

#define MIN_CAPACITY 8

Vector* vector_create(size_t initial_capacity, FreeFunc free_fn) {
    Vector* v = (Vector*)MALLOC(sizeof(Vector));
    if (!v) return NULL;
    
    if (initial_capacity < MIN_CAPACITY) initial_capacity = MIN_CAPACITY;
    
    v->items = (void**)CALLOC(initial_capacity, sizeof(void*));
    if (!v->items) {
        FREE(v);
        return NULL;
    }
    
    v->capacity = initial_capacity;
    v->size = 0;
    v->free_fn = free_fn;
    
    return v;
}

void vector_destroy(Vector* v) {
    if (!v) return;
    
    if (v->free_fn) {
        for (size_t i = 0; i < v->size; i++) {
            if (v->items[i]) {
                v->free_fn(v->items[i]);
            }
        }
    }
    
    FREE(v->items);
    FREE(v);
}

static void vector_resize(Vector* v, size_t new_capacity) {
    void** new_items = (void**)REALLOC(v->items, new_capacity * sizeof(void*));
    if (new_items) {
        v->items = new_items;
        v->capacity = new_capacity;
    }
}

void vector_push(Vector* v, void* item) {
    if (v->size == v->capacity) {
        vector_resize(v, v->capacity * 2);
    }
    
    v->items[v->size++] = item;
}

void* vector_get(Vector* v, size_t index) {
    if (index >= v->size) return NULL;
    return v->items[index];
}

void vector_set(Vector* v, size_t index, void* item) {
    if (index >= v->size) return;
    
    // If there's already an item and we have a free function, free the old one
    if (v->items[index] && v->free_fn && v->items[index] != item) {
        v->free_fn(v->items[index]);
    }
    
    v->items[index] = item;
}

void* vector_pop(Vector* v) {
    if (v->size == 0) return NULL;
    return v->items[--v->size];
}

void vector_remove(Vector* v, size_t index) {
    if (index >= v->size) return;
    
    // Free the item being removed
    if (v->items[index] && v->free_fn) {
        v->free_fn(v->items[index]);
    }
    
    // Shift elements down
    for (size_t i = index; i < v->size - 1; i++) {
        v->items[i] = v->items[i + 1];
    }
    
    v->size--;
}

void vector_remove_fast(Vector* v, size_t index) {
    if (index >= v->size) return;
    
    if (v->items[index] && v->free_fn) {
        v->free_fn(v->items[index]);
    }
    
    // Swap with last element for O(1) removal
    v->items[index] = v->items[v->size - 1];
    v->size--;
}

void vector_clear(Vector* v) {
    if (v->free_fn) {
        for (size_t i = 0; i < v->size; i++) {
            if (v->items[i]) {
                v->free_fn(v->items[i]);
            }
        }
    }
    v->size = 0;
}

void* vector_find(Vector* v, CompareFunc cmp, const void* target) {
    for (size_t i = 0; i < v->size; i++) {
        if (cmp(v->items[i], target) == 0) {
            return v->items[i];
        }
    }
    return NULL;
}

int vector_find_index(Vector* v, CompareFunc cmp, const void* target) {
    for (int i = 0; i < (int)v->size; i++) {
        if (cmp(v->items[i], target) == 0) {
            return i;
        }
    }
    return -1;
}

void vector_sort(Vector* v, CompareFunc cmp) {
    qsort(v->items, v->size, sizeof(void*), (int (*)(const void*, const void*))cmp);
}

size_t vector_size(Vector* v) {
    return v ? v->size : 0;
}

bool vector_empty(Vector* v) {
    return v ? (v->size == 0) : true;
}

void* vector_last(Vector* v) {
    if (!v || v->size == 0) return NULL;
    return v->items[v->size - 1];
}

void* vector_first(Vector* v) {
    if (!v || v->size == 0) return NULL;
    return v->items[0];
}
