#ifndef HMS_TYPES_H
#define HMS_TYPES_H

#include <stddef.h>
#include <stdbool.h>

// Result type for error handling
typedef struct {
    int success;
    int error_code;
    char error_msg[256];
} Result;

// Generic function pointer types
typedef int (*CompareFunc)(const void* a, const void* b);
typedef void (*FreeFunc)(void* item);
typedef void (*PrintFunc)(const void* item);
typedef bool (*FilterFunc)(const void* item, void* user_data);

// Common constants
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 200
#define MAX_REMARK_LENGTH 200
#define MAX_USERNAME 30
#define MAX_PASSWORD 50
#define MAX_CONTACT 20

// ANSI Color codes (reusing from original)
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#endif // HMS_TYPES_H
