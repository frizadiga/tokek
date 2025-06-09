#ifndef TUI_H
#define TUI_H

void clear_screen(void);
int get_terminal_width(void);
void print_header(void);
void display_progress_bar(int current, int total, int width);
void display_generated_tokens(int count);

#endif