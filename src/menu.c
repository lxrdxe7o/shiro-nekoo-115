#include "../include/menu.h"
#include "../include/memory.h"
#include <string.h>
#include <stdlib.h>

Menu* menu_create(const char* title, MenuItem* items, int count) {
    Menu* m = (Menu*)CALLOC(1, sizeof(Menu));
    if (!m) return NULL;
    
    // Auto-size window based on items
    int h = count + 4;
    int w = 40;
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;
    
    m->win = tui_window_create(h, w, y, x, title);
    m->title = strdup(title);
    
    // Copy items so caller can stack-allocate them
    m->items = (MenuItem*)CALLOC(count, sizeof(MenuItem));
    for (int i = 0; i < count; i++) {
        m->items[i] = items[i];
        if (items[i].label) {
            m->items[i].label = strdup(items[i].label);
        }
    }
    m->item_count = count;
    m->selected = 0;
    
    return m;
}

void menu_destroy(Menu* menu) {
    if (!menu) return;
    
    tui_window_destroy(menu->win);
    free(menu->title); // Using free because strdup used
    
    for (int i = 0; i < menu->item_count; i++) {
        free(menu->items[i].label);
    }
    FREE(menu->items);
    FREE(menu);
}

static void draw_menu(Menu* m) {
    tui_window_clear(m->win);
    
    for (int i = 0; i < m->item_count; i++) {
        if (i == m->selected) {
            wattron(m->win->win, COLOR_PAIR(COLOR_PAIR_MENU_SELECTED) | A_BOLD);
            mvwprintw(m->win->win, i + 2, 2, " > %s ", m->items[i].label);
            wattroff(m->win->win, COLOR_PAIR(COLOR_PAIR_MENU_SELECTED) | A_BOLD);
        } else {
            wattron(m->win->win, COLOR_PAIR(COLOR_PAIR_MENU_ITEM));
            mvwprintw(m->win->win, i + 2, 2, "   %s ", m->items[i].label);
            wattroff(m->win->win, COLOR_PAIR(COLOR_PAIR_MENU_ITEM));
        }
    }
    wrefresh(m->win->win);
}

int menu_run(Menu* m) {
    int ch;
    draw_menu(m);
    
    while ((ch = wgetch(m->win->win)) != KEY_ESC) {
        switch (ch) {
            case KEY_UP:
                if (m->selected > 0) m->selected--;
                else m->selected = m->item_count - 1;
                break;
            case KEY_DOWN:
                if (m->selected < m->item_count - 1) m->selected++;
                else m->selected = 0;
                break;
            case '\n':
            case KEY_ENTER:
                if (m->items[m->selected].action) {
                    m->items[m->selected].action(m->items[m->selected].user_data);
                }
                return m->items[m->selected].id;
        }
        draw_menu(m);
    }
    
    return -1; // Canceled
}
