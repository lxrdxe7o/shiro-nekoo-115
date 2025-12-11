#ifndef HMS_MENU_H
#define HMS_MENU_H

#include "tui.h"

typedef struct {
    char* label;
    int id;                 // ID to return on selection
    void (*action)(void*);  // Optional callback
    void* user_data;
} MenuItem;

typedef struct {
    TuiWindow* win;
    char* title;
    MenuItem* items;
    int item_count;
    int selected;
} Menu;

Menu* menu_create(const char* title, MenuItem* items, int count);
void  menu_destroy(Menu* menu);
int   menu_run(Menu* menu);  // Returns items[selected].id or -1 on exit

#endif // HMS_MENU_H
