#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "objects.h"
#include "tet_frontend.h"
#include "tsm.h"

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

//////////////////////  ПЕЧАТАЕМ ПОВЕРХ ЭКРАНА ////////////////////////////
void print_overlay(void) {
  print_rectangle(-1, ROWS_FIELD, -1, COLS_FIELD + 10);
  print_rectangle(-1, ROWS_FIELD, COLS_FIELD + 11, COLS_FIELD + HUD_WIDTH);
  print_rectangle(0, 2, COLS_FIELD + 12, COLS_FIELD + HUD_WIDTH - 1);
  print_rectangle(3, 5, COLS_FIELD + 12, COLS_FIELD + HUD_WIDTH - 1);
  print_rectangle(6, 8, COLS_FIELD + 12, COLS_FIELD + HUD_WIDTH - 1);
  print_rectangle(9, 11, COLS_FIELD + 12, COLS_FIELD + HUD_WIDTH - 1);
  print_rectangle(12, 17, COLS_FIELD + 12, COLS_FIELD + HUD_WIDTH - 1);
  print_rectangle(12, 17, COLS_FIELD + 20, COLS_FIELD + HUD_WIDTH - 1);
  print_rectangle(18, 20, COLS_FIELD + 12, COLS_FIELD + HUD_WIDTH - 1);

  MVPRINTW(1, COLS_FIELD + 13, "H_SCORE:");
  MVPRINTW(4, COLS_FIELD + 13, "С_SCORE:");
  MVPRINTW(7, COLS_FIELD + 13, "LEVEL:");
  MVPRINTW(10, COLS_FIELD + 13, "SPEED:");
  MVPRINTW(13, COLS_FIELD + 13, "NEXT");
  MVPRINTW(15, COLS_FIELD + 13, "BLOCK");
  MVPRINTW(19, COLS_FIELD + 13, "PAUSE:");
}

void print_stats(GameInfo_t* game_info) {
  MVPRINTW(1, COLS_FIELD + 24, "%d", game_info->high_score);
  MVPRINTW(4, COLS_FIELD + 25, "%d", game_info->score);
  MVPRINTW(7, COLS_FIELD + 24, "%d", game_info->level);
  MVPRINTW(10, COLS_FIELD + 24, "%d", game_info->speed);
  MVPRINTW(19, COLS_FIELD + 24, "%s", game_info->pause);
}

////////Записываем новую рандомную фигуру в блок next_block
void show_next_block(matrix_t updfig) {
  for (int i = 0; i < updfig.hight; i++) {
    for (int j = 0; j < updfig.widht; j++) {
      char empty = 32;
      char block = 64;
      attron(A_BOLD);
      attron(COLOR_PAIR(3));
      MVADDCH(13 + i, COLS_FIELD + 23 + j,
              (updfig.field[i][j] == 0) ? empty : block);
      attroff(COLOR_PAIR(3));
      attroff(A_BOLD);
    }
  }
}

// Создаем матрицу для создания поля и фигур :
matrix_t matrix(int hight, int widht) {
  matrix_t field_tet;
  field_tet.hight = hight;
  field_tet.widht = widht;
  for (int i = 0; i < field_tet.hight; i++) {
    for (int j = 0; j < field_tet.widht; j++) {
      field_tet.field[i][j] = 0;
    }
  }
  return field_tet;
}

///////////////////////////////////////////////
// Копируем матрицу
matrix_t copy_block(matrix_t origin_b) {
  matrix_t new_block = matrix(origin_b.hight, origin_b.widht);
  for (int i = 0; i < new_block.hight; i++) {
    for (int j = 0; j < new_block.widht; j++) {
      new_block.field[i][j] = origin_b.field[i][j];
    }
  }
  return new_block;
}

///////////////////////////////////////////////////////
//// Полная карта игры 24х22 (включает игровое поле 20х10)
matrix_t field() {
  matrix_t field_tet = matrix(ROWS_MAP, COLS_MAP);
  for (int i = 0; i < field_tet.hight; i++) {
    for (int j = 0; j < field_tet.widht; j++) {
      attron(COLOR_PAIR(3));
      field_tet.field[i][j] = 0;
      attroff(COLOR_PAIR(3));
    }
  }
  return field_tet;
}

/////////////////////////////////////////////////
// Фигура O
matrix_t figure_O() {
  matrix_t fig = matrix(2, 2);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      fig.field[i][j] = 1;
    }
  }
  return fig;
}

/////////////////////////////////////////////////
// Фигура I
matrix_t figure_I() {
  matrix_t fig = matrix(1, 4);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      fig.field[i][j] = 1;
    }
  }
  return fig;
}

/////////////////////////////////////////////////
// Фигура T
matrix_t figure_T() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (i == 1 || j == fig.hight - 1) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }
  return fig;
}

/////////////////////////////////////////////////
// Фигура L
matrix_t figure_L() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (i == 1 || j == fig.hight - 2) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }

  return fig;
}

/////////////////////////////////////////////////
// Фигура J
matrix_t figure_J() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (i == 1 || j == fig.hight) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }
  return fig;
}

/////////////////////////////////////////////////
// Фигура S
matrix_t figure_S() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if ((i == 1 && j != fig.hight) || (i == 0 && j != fig.hight - 2)) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }
  return fig;
}

/////////////////////////////////////////////////
// Фигура Z
matrix_t figure_Z() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if ((i == 1 && j != fig.hight - 2) || (i == 0 && j != fig.hight)) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }
  return fig;
}

///////////////////////// РАНДОМ //////////////////////////
/// Добавляем уже перевернутые в разных положениях фигуры
/////////////////////////////////////////////////
// Фигура J1 ( перевернутая J)
matrix_t figure_J1() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (i == 0 || j == fig.hight) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }
  return fig;
}

/////////////////////////////////////////////////
// Фигура L1 ( перевернутая L)
matrix_t figure_L1() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (i == 0 || j == fig.hight - 2) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }
  return fig;
}

/////////////////////////////////////////////////
// Фигура T1 ( перевернутая Т)
matrix_t figure_T1() {
  matrix_t fig = matrix(2, 3);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (i == 0 || j == fig.hight - 1) {
        fig.field[i][j] = 1;
      } else {
        fig.field[i][j] = 0;
      }
    }
  }
  return fig;
}

//////////////////////////////////// Создаем рандом фигур
matrix_t create_random_block() {
  matrix_t figure;
  srand(time(NULL));
  int number = rand() % 19 + 1;
  switch (number) {
    case 1:
      figure = figure_O();
      break;
    case 2:
      figure = figure_I();
      break;
    case 3:
      figure = rotate_for_r(figure_I());
      break;
    case 4:
      figure = figure_J();
      break;
    case 5:
      figure = rotate_for_r(figure_J());
      break;
    case 6:
      figure = figure_J1();
      break;
    case 7:
      figure = rotate_for_r(figure_J1());
      break;
    case 8:
      figure = figure_L();
      break;
    case 9:
      figure = rotate_for_r(figure_L());
      break;
    case 10:
      figure = figure_L1();
      break;
    case 11:
      figure = rotate_for_r(figure_L1());
      break;
    case 12:
      figure = figure_S();
      break;
    case 13:
      figure = rotate_for_r(figure_S());
      break;
    case 14:
      figure = figure_Z();
      break;
    case 15:
      figure = rotate_for_r(figure_Z());
      break;
    case 16:
      figure = figure_T();
      break;
    case 17:
      figure = rotate_for_r(figure_T());
      break;
    case 18:
      figure = figure_T1();
      break;
    case 19:
      figure = rotate_for_r(figure_T1());
      break;
  }
  return figure;
}

//////////////////////////////////
/// Добавление в поле фигуры

void add_block(matrix_t* map, player_pos block_pos, matrix_t updfig) {
  for (int i = 0; i < updfig.hight; i++) {
    for (int j = 0; j < updfig.widht; j++) {
      if (updfig.field[i][j] == 1) {
        map->field[block_pos.y + i][block_pos.x + j] = updfig.field[i][j];
      }
    }
  }
}

/////// Печатаем текущее состояние поля ////////
matrix_t updateCurrentMap(matrix_t map) {
  for (int i = 0; i < ROWS_FIELD + 2; i++) {
    for (int j = 0; j < COLS_FIELD + 1; j++) {
      char empty = 32;
      char block = 64;
      attron(A_BOLD);
      attron(COLOR_PAIR(2));
      printw("%c ", (map.field[i][j] == 0) ? empty : block);
      attroff(COLOR_PAIR(2));
      attroff(A_BOLD);
    }
    printw("\n");
  }
  return map;
}
