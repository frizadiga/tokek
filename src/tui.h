#ifndef TUI_H
#define TUI_H

void clear_screen(void);
int get_terminal_width(void);
void render_header(void);
void render_progress_bar(int current, int total, int width);
void cleanup_display_cache(void);
void render_bar_term_width();

#endif
