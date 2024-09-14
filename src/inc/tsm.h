#ifndef TSM_H
#define TSM_H

#include "defines.h"
#include "objects.h"

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Down,
  Action
} UserAction_t;

void userInput(UserAction_t action, bool hold, bool* paused, bool* terminate,
               player_pos* block_pos, matrix_t* map, matrix_t* fig, int h_score,
               int max_score);
UserAction_t getUserAction(int rc);

matrix_t rotate_for_r(matrix_t fig);
matrix_t rotate(player_pos* block_pos, matrix_t fig);
bool check_attaching(player_pos block_pos, matrix_t map, matrix_t fig);
player_pos move_left(player_pos* block_pos, matrix_t* map, matrix_t* fig);
player_pos move_right(player_pos* block_pos, matrix_t* map, matrix_t* fig);
player_pos move_down(player_pos* block_pos, matrix_t* map, matrix_t* fig);

#endif
