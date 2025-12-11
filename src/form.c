#include "../include/form.h"
#include "../include/memory.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Form* form_create(const char* title, FormField* fields, int count) {
    Form* f = (Form*)CALLOC(1, sizeof(Form));
    if (!f) return NULL;
    
    int h = count * 3 + 4;
    int w = 60;
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;
    
    f->win = tui_window_create(h, w, y, x, title);
    f->title = strdup(title);
    
    f->fields = (FormField*)CALLOC(count, sizeof(FormField));
    for (int i = 0; i < count; i++) {
        f->fields[i] = fields[i];
        if (fields[i].label) {
            f->fields[i].label = strdup(fields[i].label);
        }
    }
    f->field_count = count;
    f->current_field = 0;
    
    return f;
}

void form_destroy(Form* form) {
    if (!form) return;
    
    tui_window_destroy(form->win);
    free(form->title);
    for (int i = 0; i < form->field_count; i++) {
        free(form->fields[i].label);
    }
    FREE(form->fields);
    FREE(form);
}

static void draw_form(Form* f) {
    tui_window_clear(f->win);
    
    for (int i = 0; i < f->field_count; i++) {
        int y = i * 3 + 2;
        mvwprintw(f->win->win, y, 2, "%s:", f->fields[i].label);
        
        // Draw input box
        wattron(f->win->win, COLOR_PAIR(COLOR_PAIR_INPUT));
        mvwhline(f->win->win, y + 1, 2, ' ', 40);
        
        // Draw current value
        char display_buf[128] = {0};
        void* val = f->fields[i].value;
        
        switch (f->fields[i].type) {
            case FIELD_TEXT:
            case FIELD_PASSWORD:
                if (val && *(char*)val) {
                    strncpy(display_buf, (char*)val, 40);
                    if (f->fields[i].type == FIELD_PASSWORD) {
                        for(size_t k=0; k<strlen(display_buf); k++) display_buf[k] = '*';
                    }
                }
                break;
            case FIELD_INTEGER:
                if (*(int*)val != 0) snprintf(display_buf, 40, "%d", *(int*)val);
                break;
            case FIELD_FLOAT:
                if (*(float*)val != 0.0f) snprintf(display_buf, 40, "%.2f", *(float*)val);
                break;
        }
        
        mvwprintw(f->win->win, y + 1, 2, "%-40s", display_buf); // Left align, pad
        wattroff(f->win->win, COLOR_PAIR(COLOR_PAIR_INPUT));
        
        if (i == f->current_field) {
            mvwprintw(f->win->win, y + 1, 44, "<");
        }
    }
    
    mvwprintw(f->win->win, f->field_count * 3 + 2, 2, 
        "[ENTER] Edit   [UP/DOWN] Nav   [TAB] Submit   [ESC] Cancel");
        
    wrefresh(f->win->win);
}

static void edit_field(Form* f) {
    FormField* field = &f->fields[f->current_field];
    int y = f->current_field * 3 + 3;
    int x = 2; // Relative to window
    
    // Clear line first
    mvwprintw(f->win->win, y, x, "                                        ");
    
    char buf[128] = {0};
    
    // Pre-populate
    if (field->type == FIELD_TEXT && field->value) {
        strncpy(buf, (char*)field->value, sizeof(buf)-1);
    } else if (field->type == FIELD_INTEGER) {
        if (*(int*)field->value != 0) snprintf(buf, sizeof(buf), "%d", *(int*)field->value);
    } else if (field->type == FIELD_FLOAT) {
        if (*(float*)field->value != 0.0f) snprintf(buf, sizeof(buf), "%.2f", *(float*)field->value);
    }
    
    tui_read_line(f->win->win, y, x, buf, 
                  field->max_length > 0 ? field->max_length : 40, 
                  field->type == FIELD_PASSWORD);
                  
    // Save back
    switch (field->type) {
        case FIELD_TEXT:
        case FIELD_PASSWORD:
             // Caller expects buffer at pointer
             strcpy((char*)field->value, buf);
             break;
        case FIELD_INTEGER:
             *(int*)field->value = atoi(buf);
             break;
        case FIELD_FLOAT:
             *(float*)field->value = atof(buf);
             break;
    }
}

bool form_run(Form* f) {
    int ch;
    draw_form(f);
    
    while ((ch = wgetch(f->win->win)) != KEY_ESC) {
        switch (ch) {
            case KEY_UP:
            case 'k':
                if (f->current_field > 0) f->current_field--;
                else f->current_field = f->field_count - 1;
                break;
            case KEY_DOWN:
            case 'j':
                if (f->current_field < f->field_count - 1) f->current_field++;
                else f->current_field = 0;
                break;
            case '\n':
            case KEY_ENTER:
                edit_field(f);
                break;
            case '\t': // Tab to submit
                if (tui_confirm("Submit form?")) return true;
                // redraw needed after confirm popup
                touchwin(f->win->win);
                break;
        }
        draw_form(f);
    }
    
    return false;
}
