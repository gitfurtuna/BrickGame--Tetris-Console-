#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <ncurses.h>

#include "defines.h"
#include "objects.h"

int delete_full_line(matrix_t* map);
int set_score(int a);
int get_hight_score();
int speed(int a, int speed_0, int count);
int score(int a);
int level(int a);

#endif
