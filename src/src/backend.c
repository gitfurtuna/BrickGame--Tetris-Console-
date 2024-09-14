#include <stdio.h>
#include <stdlib.h>

#include "objects.h"
#include "tet_backend.h"
#include "tet_frontend.h"

/// Запись в файл ///
int set_score(int a) {
  FILE *f = fopen("data.bin", "wb");
  fwrite(&a, sizeof(a), 1, f);
  fclose(f);
  return a;
}

// Считывание из файла ///
int get_hight_score() {
  FILE *f = fopen("data.bin", "rb");
  int r = 0;
  if (!f)
    set_score(0);
  else {
    fread(&r, sizeof(r), 1, f);
    fclose(f);
  }
  return r;
}

/// Счет ///
int score(int a) {
  if (a == 1) {
    a = 100;
  } else if (a == 2) {
    a = 300;
  } else if (a == 3) {
    a = 700;
  } else if (a == 4) {
    a = 1500;
  }
  return a;
}

/// Повышение скорости ///
int speed(int a, int speed_0, int count) {
  int c = 10 - a;
  speed_0 = count % c;
  attron(A_BOLD);
  attron(COLOR_PAIR(7));
  MVPRINTW(10, COLS_FIELD + 23, "%d", a + 1);
  attroff(COLOR_PAIR(7));
  attroff(A_BOLD);
  return speed_0;
}

//// Повышение уровня //
int level(int a) {
  int level = a / 600;
  attron(A_BOLD);
  attron(COLOR_PAIR(2));
  MVPRINTW(7, COLS_FIELD + 23, "%d", level + 1);
  attroff(COLOR_PAIR(2));
  attroff(A_BOLD);
  if (level == 11) {
    attron(A_BOLD);
    attron(COLOR_PAIR(2));
    MVPRINTW(7, COLS_FIELD + 23, "%d", level - 1);
    attroff(COLOR_PAIR(2));
    attroff(A_BOLD);
    timeout(-1);
    attron(A_BOLD);
    attron(COLOR_PAIR(5));
    MVPRINTW(ROWS_MAP / 2 - 3, (COLS_FIELD) / 2, " YOU WON :) ");
    attroff(COLOR_PAIR(5));
    attroff(A_BOLD);
    print_overlay();
  }
  return level;
}

/// Удаление полной линии ///
int delete_full_line(matrix_t *map) {
  int sum;
  int count = 0;
  for (int i = 0; i < ROWS_FIELD + 3; i++) {
    sum = 0;
    for (int j = 0; j < COLS_FIELD + 2; j++) {
      sum += map->field[i][j];
    }
    if (sum == FULL_LINE) {
      count++;
      int a, b;
      for (a = i; a >= 1; a--)
        for (b = 0; b < COLS_FIELD + 2; b++)
          map->field[a][b] = map->field[a - 1][b];  // сдвигаем на ряд вниз
      for (b = 0; b < COLS_FIELD + 2; b++) map->field[a][b] = 0;
    }
  }
  return count;
}
