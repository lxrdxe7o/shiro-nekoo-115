#include "../include/table.h"
#include "../include/memory.h"
#include <string.h>
#include <stdlib.h>

Table* table_create(TuiWindow* win, TableColumn* cols, int count) {
    Table* t = (Table*)CALLOC(1, sizeof(Table));
    if (!t) return NULL;
    
    t->win = win;
    t->columns = (TableColumn*)CALLOC(count, sizeof(TableColumn));
    for (int i = 0; i < count; i++) {
        t->columns[i] = cols[i];
        if (cols[i].header) {
            t->columns[i].header = strdup(cols[i].header);
        }
    }
    t->column_count = count;
    t->data = NULL;
    t->scroll_offset = 0;
    t->selected_row = 0;
    
    return t;
}

void table_destroy(Table* t) {
    if (!t) return;
    
    for (int i = 0; i < t->column_count; i++) {
        free(t->columns[i].header);
    }
    FREE(t->columns);
    FREE(t);
}

void table_set_data(Table* t, Vector* data) {
    t->data = data;
    t->scroll_offset = 0;
    t->selected_row = 0;
}

static void draw_table(Table* t) {
    tui_window_clear(t->win);
    
    // Draw Header
    int x = 2;
    wattron(t->win->win, A_BOLD | A_UNDERLINE);
    for (int i = 0; i < t->column_count; i++) {
        mvwprintw(t->win->win, 2, x, "%-*s", t->columns[i].width, t->columns[i].header);
        x += t->columns[i].width + 2;
    }
    wattroff(t->win->win, A_BOLD | A_UNDERLINE);
    
    // Draw Data
    int visible_rows = t->win->height - 4;
    int count = vector_size(t->data);
    
    for (int i = 0; i < visible_rows; i++) {
        int index = t->scroll_offset + i;
        if (index >= count) break;
        
        void* item = vector_get(t->data, index);
        
        if (index == t->selected_row) {
            wattron(t->win->win, COLOR_PAIR(COLOR_PAIR_MENU_SELECTED));
        }
        
        x = 2;
        for (int c = 0; c < t->column_count; c++) {
            const char* val = t->columns[c].get_value(item, c);
            char buf[128];
            snprintf(buf, sizeof(buf), "%-*s", t->columns[c].width, val ? val : "");
            // Truncate if needed
            if ((int)strlen(buf) > t->columns[c].width) buf[t->columns[c].width] = '\0';
            
            mvwprintw(t->win->win, i + 3, x, "%s", buf);
            x += t->columns[c].width + 2;
        }
        
        if (index == t->selected_row) {
            wattroff(t->win->win, COLOR_PAIR(COLOR_PAIR_MENU_SELECTED));
        }
    }
    
    mvwprintw(t->win->win, t->win->height - 1, 2, "Rows: %d/%d  [UP/DOWN] Scroll  [ENTER] Select  [ESC] Back", 
         t->selected_row + 1, count);
         
    wrefresh(t->win->win);
}

void* table_run(Table* t) {
    int ch;
    int count = vector_size(t->data);
    int visible_rows = t->win->height - 4;
    
    if (count == 0) {
        tui_message("Info", "No data to display", COLOR_PAIR_NORMAL);
        return NULL;
    }
    
    draw_table(t);
    
    while ((ch = wgetch(t->win->win)) != KEY_ESC) {
        switch (ch) {
            case KEY_UP:
                if (t->selected_row > 0) {
                    t->selected_row--;
                    if (t->selected_row < t->scroll_offset) {
                        t->scroll_offset--;
                    }
                }
                break;
            case KEY_DOWN:
                if (t->selected_row < count - 1) {
                    t->selected_row++;
                    if (t->selected_row >= t->scroll_offset + visible_rows) {
                        t->scroll_offset++;
                    }
                }
                break;
            case '\n':
            case KEY_ENTER:
                return vector_get(t->data, t->selected_row);
        }
        draw_table(t);
    }
    
    return NULL;
}
