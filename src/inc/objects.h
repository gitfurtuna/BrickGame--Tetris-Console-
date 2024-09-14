#ifndef OBJECTS_H
#define OBJECTS_H

#include <ncurses.h>

#include "defines.h"

/// Позиция блока ///
typedef struct {
  int x;
  int y;
} player_pos;

/// Текущее состояние игры ///
typedef struct {
  // matrix_t field;
  // matrix_t next;
  int high_score;
  int score;
  int level;
  int speed;
  char* pause;
} GameInfo_t;

/// Структура для создания поля и фигур ///
typedef struct matrix_struct {
  int field[MAX_HEIGHT][MAX_WIDTH];
  int hight;
  int widht;
} matrix_t;

#endif
