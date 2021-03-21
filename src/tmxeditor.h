#ifndef TMXEDITOR_H
#define TMXEDITOR_H

#define TEXT_LENGTH 20000
#ifndef LINE_LENGTH
#define LINE_LENGTH 64
#endif
#define LAST_POS_IN_LINE (LINE_LENGTH - 1)

extern unsigned char text[TEXT_LENGTH];
extern unsigned char *line;
extern char cx, cxend;
extern int end, realend;
extern int y, yend;
extern int changed_one_line;
extern int changed_lines;
extern int changed_pos;

void left(void);
void right(void);
int add(unsigned char ch);
void backspace(void);
void del(void);
void enter(void);
void up(void);
void down(void);
void page_up(void);
void page_down(void);
void home(void);
void kbend(void);
void delete_line(void);
void cursor_postion(void);

unsigned char *find_nth_line(unsigned char *begin, unsigned int n, unsigned int *res);

#endif
