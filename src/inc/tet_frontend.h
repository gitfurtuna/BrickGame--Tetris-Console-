#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H

#include "defines.h"
#include "objects.h"

void print_overlay(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void add_block(matrix_t *map, player_pos block_pos, matrix_t updfig);
void show_next_block(matrix_t updfig);
matrix_t matrix(int hight, int widht);
matrix_t copy_block(matrix_t origin_b);
matrix_t field();
matrix_t figure_O();
matrix_t figure_I();
matrix_t figure_T();
matrix_t figure_L();
matrix_t figure_J();
matrix_t figure_S();
matrix_t figure_Z();
matrix_t figure_J1();
matrix_t figure_L1();
matrix_t figure_T1();
matrix_t create_random_block();
matrix_t updateCurrentMap(matrix_t map);

#endif
