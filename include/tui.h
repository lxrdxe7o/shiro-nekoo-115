#ifndef HMS_TUI_H
#define HMS_TUI_H

#include <ncurses.h>
#include <stdbool.h>

// Colors
typedef enum {
    COLOR_PAIR_NORMAL = 1,
    COLOR_PAIR_HEADER,
    COLOR_PAIR_MENU_ITEM,
    COLOR_PAIR_MENU_SELECTED,
    COLOR_PAIR_SUCCESS,
    COLOR_PAIR_ERROR,
    COLOR_PAIR_WARNING,
    COLOR_PAIR_INPUT,
    COLOR_PAIR_BORDER,
    COLOR_PAIR_DISABLED
} TuiColor;

// Window abstraction
typedef struct {
    WINDOW* win;
    WINDOW* shadow;
    int x, y, width, height;
    char* title;
} TuiWindow;

// Core
void tui_init(void);
void tui_shutdown(void);
void tui_refresh_all(void);
void tui_clear_screen(void);
void tui_set_background_title(const char* title);

// Themes
typedef enum {
    THEME_GRUVBOX,
    THEME_MONOKAI,
    THEME_BAMBOO
} TuiTheme;

typedef enum {
    ART_MAIN,
    ART_PATIENT,
    ART_DOCTOR,
    ART_WARD
} TuiArtType;

void tui_set_theme(TuiTheme theme);
void tui_draw_logo(TuiArtType type);

// Window Management
TuiWindow* tui_window_create(int h, int w, int y, int x, const char* title);
void       tui_window_destroy(TuiWindow* win);
void       tui_window_draw_box(TuiWindow* win, TuiColor border_color);
void       tui_window_clear(TuiWindow* win);

// Input handling
int  tui_get_key(void);
void tui_read_line(WINDOW* win, int y, int x, char* buf, int max_len, bool mask);
int  tui_read_int(WINDOW* win, int y, int x);
float tui_read_float(WINDOW* win, int y, int x);
bool tui_confirm(const char* message);
void tui_message(const char* title, const char* msg, TuiColor color);
void tui_error(const char* msg);
void tui_success(const char* msg);

// Key codes abstraction
#define KEY_ENTER_CUSTOM 10
#define KEY_ESC 27
#define KEY_TAB 9

#endif // HMS_TUI_H
