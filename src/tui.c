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
    use_default_colors();
    
    // Define palette (Gruvbox Dark approximation)
    // NORMAL: White text on Default background
    init_pair(COLOR_PAIR_NORMAL, COLOR_WHITE, -1);
    
    // HEADER: Accent text on Default background
    init_pair(COLOR_PAIR_HEADER, COLOR_YELLOW, -1);
    
    // MENU ITEM: White text on Default background
    init_pair(COLOR_PAIR_MENU_ITEM, COLOR_WHITE, -1);
    
    // SELECTED: Black text on Accent background (Keep solid)
    init_pair(COLOR_PAIR_MENU_SELECTED, COLOR_BLACK, COLOR_YELLOW);
    
    // STATUS/MESSAGES
    init_pair(COLOR_PAIR_SUCCESS, COLOR_GREEN, -1);
    init_pair(COLOR_PAIR_ERROR, COLOR_RED, -1);
    init_pair(COLOR_PAIR_WARNING, COLOR_YELLOW, -1);
    
    // INPUT: Black text on Accent background (Solid block)
    init_pair(COLOR_PAIR_INPUT, COLOR_BLACK, COLOR_YELLOW);
    
    // BORDER: White (or Gray) on Default
    init_pair(COLOR_PAIR_BORDER, COLOR_WHITE, -1);
    
    init_pair(COLOR_PAIR_DISABLED, COLOR_MAGENTA, -1);
    
    // Shadow color (Black on Default?)
    // Using -1 on -1 makes it invisible if terminal bg is same.
    // Let's keep shadow as actual COLOR_BLACK block if bg is -1?
    // Or make shadow -1, -1 for "transparent"? 
    // User wants "pitch black" - implies the grey block they saw should be black.
    // If we use COLOR_BLACK, -1 it might be just text?
    // Let's make Shadow defined as just empty space with default bg?
    // Actually, simply remove shadow color pair usage or set to -1,-1
    init_pair(20, -1, -1); 
    
    bkgd(COLOR_PAIR(COLOR_PAIR_NORMAL));
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

// Theme Management
void tui_set_theme(TuiTheme theme) {
    short accent_color = COLOR_YELLOW;
    
    switch (theme) {
        case THEME_GRUVBOX: accent_color = COLOR_YELLOW; break;
        case THEME_MONOKAI: accent_color = COLOR_MAGENTA; break;
        case THEME_BAMBOO:  accent_color = COLOR_GREEN; break;
        case THEME_CORAL:   accent_color = COLOR_RED; break;
    }
    
    // Redefine pairs based on accent
    init_pair(COLOR_PAIR_HEADER, accent_color, -1);
    
    // SELECTED: Black text on Accent background (SOLID)
    init_pair(COLOR_PAIR_MENU_SELECTED, COLOR_BLACK, accent_color);
    
    // INPUT: Black text on Accent background (SOLID)
    init_pair(COLOR_PAIR_INPUT, COLOR_BLACK, accent_color);
    
    // Refresh to apply changes immediately
    refresh();
}

void tui_draw_logo(TuiArtType type) {
    int y = 4;
    attron(COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);

    if (type == ART_MAIN) {
        int x = (COLS - 64) / 2;
        if (x < 0) x = 0;
        mvprintw(y++, x, " ██╗  ██╗ ██████╗ ███████╗██████╗ ██╗████████╗ █████╗ ██╗     ");
        mvprintw(y++, x, " ██║  ██║██╔═══██╗██╔════╝██╔══██╗██║╚══██╔══╝██╔══██╗██║     ");
        mvprintw(y++, x, " ███████║██║   ██║███████╗██████╔╝██║   ██║   ███████║██║     ");
        mvprintw(y++, x, " ██╔══██║██║   ██║╚════██║██╔═══╝ ██║   ██║   ██╔══██║██║     ");
        mvprintw(y++, x, " ██║  ██║╚██████╔╝███████║██║     ██║   ██║   ██║  ██║███████╗");
        mvprintw(y++, x, " ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚═╝  ╚═╝╚══════╝");
    } else if (type == ART_PATIENT) {
        int x = (COLS - 60) / 2; // Approx width
        if (x < 0) x = 0;
        mvprintw(y++, x, " ██████╗  █████╗ ████████╗██╗███████╗███╗   ██╗████████╗███████╗");
        mvprintw(y++, x, " ██╔══██╗██╔══██╗╚══██╔══╝██║██╔════╝████╗  ██║╚══██╔══╝██╔════╝");
        mvprintw(y++, x, " ██████╔╝███████║   ██║   ██║█████╗  ██╔██╗ ██║   ██║   ███████╗");
        mvprintw(y++, x, " ██╔═══╝ ██╔══██║   ██║   ██║██╔══╝  ██║╚██╗██║   ██║   ╚════██║");
        mvprintw(y++, x, " ██║     ██║  ██║   ██║   ██║███████╗██║ ╚████║   ██║   ███████║");
        mvprintw(y++, x, " ╚═╝     ╚═╝  ╚═╝   ╚═╝   ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝");
    } else if (type == ART_DOCTOR) {
        int x = (COLS - 56) / 2;
        if (x < 0) x = 0;
        mvprintw(y++, x, " ██████╗  ██████╗  ██████╗████████╗ ██████╗ ██████╗ ███████╗");
        mvprintw(y++, x, " ██╔══██╗██╔═══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔══██╗██╔════╝");
        mvprintw(y++, x, " ██║  ██║██║   ██║██║        ██║   ██║   ██║██████╔╝███████╗");
        mvprintw(y++, x, " ██║  ██║██║   ██║██║        ██║   ██║   ██║██╔══██╗╚════██║");
        mvprintw(y++, x, " ██████╔╝╚██████╔╝╚██████╗   ██║   ╚██████╔╝██║  ██║███████║");
        mvprintw(y++, x, " ╚═════╝  ╚═════╝  ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝");
    } else if (type == ART_WARD) {
        int x = (COLS - 40) / 2;
        if (x < 0) x = 0;
        mvprintw(y++, x, " ██╗    ██╗ █████╗ ██████╗ ██████╗ ");
        mvprintw(y++, x, " ██║    ██║██╔══██╗██╔══██╗██╔══██╗");
        mvprintw(y++, x, " ██║ █╗ ██║███████║██████╔╝██║  ██║");
        mvprintw(y++, x, " ██║███╗██║██╔══██║██╔══██╗██║  ██║");
        mvprintw(y++, x, " ╚███╔███╔╝██║  ██║██║  ██║██████╔╝");
        mvprintw(y++, x, "  ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ ");
    }

    attroff(COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    refresh();
}

void tui_set_background_title(const char* title) {
    attron(COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    mvhline(0, 0, ' ', COLS);
    mvprintw(0, (COLS - strlen(title)) / 2, "%s", title);
    mvprintw(0, COLS - 15, "v2.0 Refactored");
    attroff(COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    
    // Draw footer hint
    attron(COLOR_PAIR(COLOR_PAIR_HEADER)); /* Use header color for footer too */
    mvhline(LINES - 1, 0, ' ', COLS);
    mvprintw(LINES - 1, 2, " [j/k] Nav  [Enter] Select  [Esc] Back  [h/l] Confirm ");
    attroff(COLOR_PAIR(COLOR_PAIR_HEADER));
    
    refresh();
}

TuiWindow* tui_window_create(int h, int w, int y, int x, const char* title) {
    TuiWindow* win = (TuiWindow*)MALLOC(sizeof(TuiWindow));
    if (!win) return NULL;
    
    if (h == 0) h = LINES;
    if (w == 0) w = COLS;
    
    // Create shadow first
    win->shadow = newwin(h, w, y + 1, x + 2);
    wbkgd(win->shadow, COLOR_PAIR(20)); // Shadow color
    
    win->win = newwin(h, w, y, x);
    win->height = h;
    win->width = w;
    win->y = y;
    win->x = x;
    win->title = safe_strdup(title);
    
    // Set background for main window
    wbkgd(win->win, COLOR_PAIR(COLOR_PAIR_NORMAL));
    
    keypad(win->win, TRUE);
    tui_window_draw_box(win, COLOR_PAIR_BORDER);
    
    return win;
}

void tui_window_destroy(TuiWindow* win) {
    if (!win) return;
    
    if (win->win) {
        delwin(win->win);
    }
    if (win->shadow) {
        delwin(win->shadow);
    }
    FREE(win->title);
    FREE(win);
}

void tui_window_draw_box(TuiWindow* win, TuiColor border_color) {
    if (win->shadow) {
        wnoutrefresh(win->shadow);
    }

    // Draw Double Border Manually
    wattron(win->win, COLOR_PAIR(border_color) | A_BOLD);
    
    int w = win->width;
    int h = win->height;

    // Corners
    mvwprintw(win->win, 0, 0, "╔");
    mvwprintw(win->win, 0, w - 1, "╗");
    mvwprintw(win->win, h - 1, 0, "╚");
    mvwprintw(win->win, h - 1, w - 1, "╝");

    // Sides
    for (int i = 1; i < w - 1; i++) {
        mvwprintw(win->win, 0, i, "═");
        mvwprintw(win->win, h - 1, i, "═");
    }
    for (int i = 1; i < h - 1; i++) {
        mvwprintw(win->win, i, 0, "║");
        mvwprintw(win->win, i, w - 1, "║");
    }

    wattroff(win->win, COLOR_PAIR(border_color) | A_BOLD);
    
    if (win->title) {
        wattron(win->win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
        mvwprintw(win->win, 0, 2, " %s ", win->title);
        wattroff(win->win, COLOR_PAIR(COLOR_PAIR_HEADER) | A_BOLD);
    }
    
    wnoutrefresh(win->win);
    doupdate();
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
        if (ch == KEY_LEFT || ch == KEY_RIGHT || ch == 'h' || ch == 'l') {
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
