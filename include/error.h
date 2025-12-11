#ifndef HMS_ERROR_H
#define HMS_ERROR_H

typedef enum {
    HMS_OK = 0,
    HMS_ERR_MEMORY,
    HMS_ERR_FILE_NOT_FOUND,
    HMS_ERR_FILE_CORRUPT,
    HMS_ERR_INVALID_INPUT,
    HMS_ERR_NOT_FOUND,
    HMS_ERR_DUPLICATE,
    HMS_ERR_CAPACITY_FULL,
    HMS_ERR_AUTH_FAILED,
    HMS_ERR_UNKNOWN
} HmsError;

void hms_set_error(HmsError code, const char* fmt, ...);
HmsError hms_get_last_error(void);
const char* hms_get_last_error_msg(void);
const char* hms_error_string(HmsError e);
void hms_clear_error(void);

// Simple logging
void hms_log(int level, const char* fmt, ...);

#endif // HMS_ERROR_H
