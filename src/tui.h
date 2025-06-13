#ifndef TUI_H
#define TUI_H

void clear_screen(void);
void move_cursor_home(void);
void clear_from_cursor(void);
int get_terminal_width(void);
int get_progress_bar_width(void);
void render_header(void);
void render_progress_bar(int current, int total, int width, char *buffer);
void cleanup_display_cache(void);
void render_bar_term_width(void);
void init_display_cache(void);

#endif
