#ifndef HMS_FORM_H
#define HMS_FORM_H

#include "tui.h"

typedef enum {
    FIELD_TEXT,
    FIELD_PASSWORD,
    FIELD_INTEGER,
    FIELD_FLOAT
} FieldType;

typedef struct {
    char* label;
    FieldType type;
    void* value;            // Pointer to target variable string/int/float
    int max_length;         // For text
} FormField;

typedef struct {
    TuiWindow* win;
    char* title;
    FormField* fields;
    int field_count;
    int current_field;
} Form;

Form* form_create(const char* title, FormField* fields, int count);
void  form_destroy(Form* form);
bool  form_run(Form* form);  // Returns true if submitted, false if cancelled

#endif // HMS_FORM_H
