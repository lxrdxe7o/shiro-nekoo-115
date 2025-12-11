#include "../include/error.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static struct {
    HmsError code;
    char message[512];
} last_error = {HMS_OK, ""};

void hms_set_error(HmsError code, const char* fmt, ...) {
    last_error.code = code;
    
    va_list args;
    va_start(args, fmt);
    vsnprintf(last_error.message, sizeof(last_error.message), fmt, args);
    va_end(args);
    
    // Auto-log errors
    hms_log(2, "ERROR [%d]: %s", code, last_error.message);
}

HmsError hms_get_last_error(void) {
    return last_error.code;
}

const char* hms_get_last_error_msg(void) {
    return last_error.message;
}

void hms_clear_error(void) {
    last_error.code = HMS_OK;
    last_error.message[0] = '\0';
}

const char* hms_error_string(HmsError e) {
    switch (e) {
        case HMS_OK: return "Success";
        case HMS_ERR_MEMORY: return "Memory allocation failed";
        case HMS_ERR_FILE_NOT_FOUND: return "File not found";
        case HMS_ERR_FILE_CORRUPT: return "File corrupted";
        case HMS_ERR_INVALID_INPUT: return "Invalid input";
        case HMS_ERR_NOT_FOUND: return "Item not found";
        case HMS_ERR_DUPLICATE: return "Duplicate item";
        case HMS_ERR_CAPACITY_FULL: return "Capacity full";
        case HMS_ERR_AUTH_FAILED: return "Authentication failed";
        default: return "Unknown error";
    }
}

void hms_log(int level, const char* fmt, ...) {
    // 0=DEBUG, 1=INFO, 2=ERROR
    const char* prefix[] = {"[DEBUG]", "[INFO] ", "[ERROR]"};
    if (level < 0 || level > 2) level = 1;
    
    // In TUI apps, stdout goes to the screen, so logging should ideally go to a file
    // For now, valid logging often goes to stderr in typical unixy ways, but 
    // since we'll have a TUI, we might want to log to a file eventually.
    // For now printing to stderr is safer than stdout which might corrupt TUI.
    
    fprintf(stderr, "%s ", prefix[level]);
    
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    
    fprintf(stderr, "\n");
}
