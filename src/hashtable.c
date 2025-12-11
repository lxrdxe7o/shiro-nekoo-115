#include "../include/hashtable.h"
#include "../include/memory.h"
#include <stdlib.h>
#include <string.h>

unsigned long hash_string(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

HashTable* hashtable_create(size_t bucket_count, FreeFunc value_free_fn) {
    HashTable* ht = (HashTable*)MALLOC(sizeof(HashTable));
    if (!ht) return NULL;
    
    ht->buckets = (HashNode**)CALLOC(bucket_count, sizeof(HashNode*));
    if (!ht->buckets) {
        FREE(ht);
        return NULL;
    }
    
    ht->bucket_count = bucket_count;
    ht->size = 0;
    ht->value_free_fn = value_free_fn;
    
    return ht;
}

void hashtable_destroy(HashTable* ht) {
    if (!ht) return;
    
    hashtable_clear(ht);
    FREE(ht->buckets);
    FREE(ht);
}

void hashtable_put(HashTable* ht, const char* key, void* value) {
    unsigned long index = hash_string(key) % ht->bucket_count;
    
    // Check if key already exists, if so replace value
    HashNode* node = ht->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (ht->value_free_fn && node->value != value) {
                ht->value_free_fn(node->value);
            }
            node->value = value;
            return;
        }
        node = node->next;
    }
    
    // Insert new node at head of bucket
    HashNode* new_node = (HashNode*)MALLOC(sizeof(HashNode));
    new_node->key = strdup(key); // Should use MALLOC wrapper for strdup really, but stdlib is fine for now
    new_node->value = value;
    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    ht->size++;
}

void* hashtable_get(HashTable* ht, const char* key) {
    unsigned long index = hash_string(key) % ht->bucket_count;
    HashNode* node = ht->buckets[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

void* hashtable_remove(HashTable* ht, const char* key) {
    unsigned long index = hash_string(key) % ht->bucket_count;
    HashNode* node = ht->buckets[index];
    HashNode* prev = NULL;
    
    while (node) {
        if (strcmp(node->key, key) == 0) {
            void* value = node->value;
            
            if (prev) {
                prev->next = node->next;
            } else {
                ht->buckets[index] = node->next;
            }
            
            // Standard free specifically for the key copy
            // In a stricter system we'd track this too
            free(node->key); 
            FREE(node);
            ht->size--;
            return value;
        }
        prev = node;
        node = node->next;
    }
    return NULL;
}

bool hashtable_contains(HashTable* ht, const char* key) {
    return hashtable_get(ht, key) != NULL;
}

void hashtable_clear(HashTable* ht) {
    for (size_t i = 0; i < ht->bucket_count; i++) {
        HashNode* node = ht->buckets[i];
        while (node) {
            HashNode* next = node->next;
            
            if (ht->value_free_fn) {
                ht->value_free_fn(node->value);
            }
            free(node->key);
            FREE(node);
            
            node = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->size = 0;
}

size_t hashtable_size(HashTable* ht) {
    return ht ? ht->size : 0;
}
