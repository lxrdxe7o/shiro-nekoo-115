#include "../include/memory.h"
#include "../include/types.h"
#include "../include/error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Global memory statistics
static struct {
    size_t total_allocated;
    size_t total_freed;
    size_t current_usage;
    size_t peak_usage;
    size_t allocation_count;
} mem_stats = {0, 0, 0, 0, 0};

// Header for tracking allocations
typedef struct MemHeader {
    size_t size;
    const char* file;
    int line;
    struct MemHeader* next;
    struct MemHeader* prev;
    // Magic number to detect buffer overflows/corruption
    unsigned int magic; 
} MemHeader;

#define MAGIC_NUMBER 0xDEADBEEF

static MemHeader* head = NULL;

static void track_allocation(MemHeader* header) {
    header->magic = MAGIC_NUMBER;
    header->next = head;
    header->prev = NULL;
    
    if (head) {
        head->prev = header;
    }
    head = header;
    
    mem_stats.total_allocated += header->size;
    mem_stats.current_usage += header->size;
    mem_stats.allocation_count++;
    
    if (mem_stats.current_usage > mem_stats.peak_usage) {
        mem_stats.peak_usage = mem_stats.current_usage;
    }
}

static void untrack_allocation(MemHeader* header) {
    if (header->magic != MAGIC_NUMBER) {
        fprintf(stderr, "MEMORY CORRUPTION DETECTED at %p!\n", (void*)(header + 1));
        abort();
    }
    
    if (header->prev) {
        header->prev->next = header->next;
    } else {
        head = header->next;
    }
    
    if (header->next) {
        header->next->prev = header->prev;
    }
    
    mem_stats.total_freed += header->size;
    mem_stats.current_usage -= header->size;
}

void* hms_malloc(size_t size, const char* file, int line) {
    if (size == 0) return NULL;
    
    size_t total_size = sizeof(MemHeader) + size;
    MemHeader* header = (MemHeader*)malloc(total_size);
    
    if (!header) {
        fprintf(stderr, "Out of memory allocating %zu bytes at %s:%d\n", size, file, line);
        // We'll trust the caller to handle NULL or abort, but for this app aborting is safer
        return NULL;
    }
    
    header->size = size;
    header->file = file;
    header->line = line;
    
    track_allocation(header);
    
    return (void*)(header + 1);
}

void* hms_calloc(size_t count, size_t size, const char* file, int line) {
    size_t total_bytes = count * size;
    void* ptr = hms_malloc(total_bytes, file, line);
    if (ptr) {
        memset(ptr, 0, total_bytes);
    }
    return ptr;
}

void* hms_realloc(void* ptr, size_t size, const char* file, int line) {
    if (!ptr) return hms_malloc(size, file, line);
    if (size == 0) {
        hms_free(ptr);
        return NULL;
    }
    
    MemHeader* header = (MemHeader*)ptr - 1;
    
    untrack_allocation(header);
    
    size_t total_size = sizeof(MemHeader) + size;
    MemHeader* new_header = (MemHeader*)realloc(header, total_size);
    
    if (!new_header) {
        // Restore/re-track the old block if realloc failed
        track_allocation(header);
        return NULL;
    }
    
    new_header->size = size;
    new_header->file = file;
    new_header->line = line;
    track_allocation(new_header);
    
    return (void*)(new_header + 1);
}

void hms_free(void* ptr) {
    if (!ptr) return;
    
    MemHeader* header = (MemHeader*)ptr - 1;
    untrack_allocation(header);
    
    // Poison memory to catch use-after-free
    memset(header, 0xCC, sizeof(MemHeader) + header->size);
    
    free(header);
}

void hms_memory_stats(size_t* allocated, size_t* freed, size_t* peak) {
    if (allocated) *allocated = mem_stats.total_allocated;
    if (freed) *freed = mem_stats.total_freed;
    if (peak) *peak = mem_stats.peak_usage;
}

void hms_memory_leak_check(void) {
    if (head == NULL) {
        // No leaks
        return;
    }
    
    fprintf(stderr, "\n=== LEAK CHECK REPORT ===\n");
    fprintf(stderr, "Total leaks: %zu bytes\n", mem_stats.current_usage);
    
    MemHeader* current = head;
    while (current) {
        fprintf(stderr, "LEAK: %zu bytes allocated at %s:%d\n", 
                current->size, current->file, current->line);
        current = current->next;
    }
    fprintf(stderr, "=========================\n");
}
