#include "../include/tui.h"
#include "../include/memory.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static char* safe_strdup(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src) + 1;
    char* dest = (char*)MALLOC(len);
    if (dest) memcpy(dest, src, len);
    return dest;
}

void tui_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor
    start_color();
    
    // Define palette
    init_pair(COLOR_PAIR_NORMAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_HEADER, COLOR_BLACK, COLOR_CYAN);
    init_pair(COLOR_PAIR_MENU_ITEM, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_MENU_SELECTED, COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_PAIR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_ERROR, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_WARNING, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_INPUT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_BORDER, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_PAIR_DISABLED, COLOR_BLACK, COLOR_WHITE);
    
    refresh();
}

void tui_shutdown(void) {
    endwin();
}

void tui_refresh_all(void) {
    refresh();
}

void tui_clear_screen(void) {
    clear();
    refresh();
}

TuiWindow* tui_window_create(int h, int w, int y, int x, const char* title) {
    TuiWindow* win = (TuiWindow*)MALLOC(sizeof(TuiWindow));
    if (!win) return NULL;
    
    if (h == 0) h = LINES;
    if (w == 0) w = COLS;
    
    win->win = newwin(h, w, y, x);
    win->height = h;
    win->width = w;
    win->y = y;
    win->x = x;
    win->title = safe_strdup(title);
    
    keypad(win->win, TRUE);
    tui_window_draw_box(win, COLOR_PAIR_BORDER);
    
    return win;
}

void tui_window_destroy(TuiWindow* win) {
    if (!win) return;
    
    if (win->win) {
        delwin(win->win);
    }
    FREE(win->title);
    FREE(win);
}

void tui_window_draw_box(TuiWindow* win, TuiColor border_color) {
    wattron(win->win, COLOR_PAIR(border_color));
    box(win->win, 0, 0);
    wattroff(win->win, COLOR_PAIR(border_color));
    
    if (win->title) {
        wattron(win->win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
        mvwprintw(win->win, 0, 2, " %s ", win->title);
        wattroff(win->win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    }
    
    wrefresh(win->win);
}

void tui_window_clear(TuiWindow* win) {
    werase(win->win);
    tui_window_draw_box(win, COLOR_PAIR_BORDER);
}

int tui_get_key(void) {
    return getch();
}

void tui_read_line(WINDOW* win, int y, int x, char* buf, int max_len, bool mask) {
    int pos = 0;
    int ch;
    
    // Initial draw (if buf has content)
    int len = (int)strlen(buf);
    if (len > 0) pos = len;

    curs_set(1); // Show cursor
    wmove(win, y, x + pos);
    wrefresh(win);
    
    while ((ch = wgetch(win)) != KEY_ENTER_CUSTOM && ch != '\n' && ch != '\r') {
        if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (pos > 0) {
                pos--;
                buf[pos] = '\0';
                mvwprintw(win, y, x + pos, " ");
                wmove(win, y, x + pos);
            }
        } else if (ch == KEY_ESC) {
            // Cancel input? For now just break or maybe clear
            // Let's assume ESC aborts or clears? 
            // Better behavior is to just return what we have or empty
            // Implementing standard text input behavior
             break;
        } else if (isprint(ch)) {
            if (pos < max_len - 1) {
                buf[pos] = ch;
                pos++;
                buf[pos] = '\0';
                if (mask) {
                    mvwprintw(win, y, x + pos - 1, "*");
                } else {
                    mvwprintw(win, y, x + pos - 1, "%c", ch);
                }
            }
        }
        wrefresh(win);
    }
    
    curs_set(0); // Hide cursor
}

int tui_read_int(WINDOW* win, int y, int x) {
    char buf[32] = {0};
    tui_read_line(win, y, x, buf, 31, false);
    return atoi(buf);
}

float tui_read_float(WINDOW* win, int y, int x) {
    char buf[32] = {0};
    tui_read_line(win, y, x, buf, 31, false);
    return atof(buf);
}

bool tui_confirm(const char* message) {
    int h = 6, w = 50;
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;
    
    WINDOW* win = newwin(h, w, y, x);
    box(win, 0, 0);
    keypad(win, TRUE);
    
    mvwprintw(win, 1, 2, "%s", message);
    mvwprintw(win, 3, 15, " [ YES ]    no ");
    wrefresh(win);
    
    bool result = true;
    int ch;
    while ((ch = wgetch(win)) != KEY_ENTER_CUSTOM && ch != '\n' && ch != '\r') {
        if (ch == KEY_LEFT || ch == KEY_RIGHT) {
            result = !result;
            if (result) {
                wattron(win, A_BOLD);
                mvwprintw(win, 3, 15, " [ YES ]    no ");
                wattroff(win, A_BOLD);
            } else {
                wattron(win, A_BOLD);
                mvwprintw(win, 3, 15, "   yes    [ NO ] ");
                wattroff(win, A_BOLD);
            }
            wrefresh(win);
        }
    }
    
    delwin(win);
    redrawwin(stdscr); // Fix background
    refresh();
    return result;
}

void tui_message(const char* title, const char* msg, TuiColor color) {
    int h = 8, w = 60;
    int y = (LINES - h) / 2;
    int x = (COLS - w) / 2;
    
    WINDOW* win = newwin(h, w, y, x);
    wattron(win, COLOR_PAIR(color));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(color));
    
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 2, " %s ", title);
    wattroff(win, A_BOLD);
    
    mvwprintw(win, 2, 2, "%s", msg);
    mvwprintw(win, 6, (w - 18)/2, "Press any key...");
    
    wrefresh(win);
    wgetch(win);
    delwin(win);
    
    redrawwin(stdscr);
    refresh();
}

void tui_error(const char* msg) {
    tui_message("Error", msg, COLOR_PAIR_ERROR);
}

void tui_success(const char* msg) {
    tui_message("Success", msg, COLOR_PAIR_SUCCESS);
}
