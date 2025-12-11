#ifndef HMS_HASHTABLE_H
#define HMS_HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>
#include "types.h"

typedef struct HashNode {
    char* key;
    void* value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** buckets;
    size_t bucket_count;
    size_t size;
    FreeFunc value_free_fn;
} HashTable;

// Lifecycle
HashTable* hashtable_create(size_t bucket_count, FreeFunc value_free_fn);
void       hashtable_destroy(HashTable* ht);

// Operations
void       hashtable_put(HashTable* ht, const char* key, void* value);
void*      hashtable_get(HashTable* ht, const char* key);
void*      hashtable_remove(HashTable* ht, const char* key);
bool       hashtable_contains(HashTable* ht, const char* key);
void       hashtable_clear(HashTable* ht);

// Accessor
size_t     hashtable_size(HashTable* ht);

// Utilities
unsigned long hash_string(const char* str);

#endif // HMS_HASHTABLE_H
