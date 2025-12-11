#ifndef HMS_TABLE_H
#define HMS_TABLE_H

#include "tui.h"
#include "vector.h"

typedef struct {
    char* header;
    int width;
    // Callback to get string for a specific item and column
    // Returns a pointer to a static/temp buffer (caller duplicates if needed)
    const char* (*get_value)(void* item, int col);
} TableColumn;

typedef struct {
    TuiWindow* win;
    TableColumn* columns;
    int column_count;
    Vector* data;
    int scroll_offset;
    int selected_row;
} Table;

Table* table_create(TuiWindow* win, TableColumn* cols, int count);
void   table_set_data(Table* t, Vector* data);
void*  table_run(Table* t);  // Returns selected item or NULL
void   table_destroy(Table* t);

#endif // HMS_TABLE_H
