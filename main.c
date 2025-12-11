#include "include/app.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

// Signal handler for graceful shutdown
void handle_signal(int sig) {
    (void)sig;
    // We rely on app_shutdown being safe to call or we set a flag
    // But since ncurses takes over the terminal, we should try to exit cleanly
    // Ideally app.c would expose a "stop" function
    // For now we just force a clean shutdown of TUI
    app_shutdown();
    exit(0);
}

int main(void) {
    // Set locale for unicode support (box drawing characters)
    setlocale(LC_ALL, "");
    
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    app_init();
    app_run();
    app_shutdown();
    
    return 0;
}
