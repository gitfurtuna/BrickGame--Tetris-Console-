#ifndef DEFINES_H
#define DEFINES_H

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define MAX_HEIGHT 100
#define MAX_WIDTH 100

#define FULL_LINE 10

#define SUCCESS 0
#define ERROR 1

#define ROWS_MAP 24
#define COLS_MAP 22
#define ROWS_FIELD 20
#define COLS_FIELD 10

#define FIELD_BEGIN 2

#define GET_USER_INPUT getch()

#define MVPRINTW(y, x, ...) \
  mvprintw(FIELD_BEGIN + (y), FIELD_BEGIN + (x), __VA_ARGS__)
#define CLEAR_BACKPOS(y, x) mvaddch(FIELD_BEGIN + (y), FIELD_BEGIN + (x), ' ')
#define MVADDCH(y, x, c) mvaddch(FIELD_BEGIN + (y), FIELD_BEGIN + (x), c)

#define MAX_WIN_COUNT 600

#define LEVEL_CNT 10
#define LEVELNAME_MAX 25

#define BLOCKSTART_X random() % 7 + 1
#define BLOCKSTART_Y 1
#define INTRO_MESSAGE "Press ENTER to START"
#define PAUSE " YES "
#define INTRO_MESSAGE_LEN 21
#define HUD_WIDTH 30

#define BANNER_N 10
#define BANNER_M 100

#define ESCAPE 27
#define ENTER_KEY 10
#define SPACE 32

#endif
