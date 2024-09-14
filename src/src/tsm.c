#include "tsm.h"

#include "tet_backend.h"
#include "tet_frontend.h"

//// Вращение фигуры для рандома ///////////////////
matrix_t rotate_for_r(matrix_t fig) {
  matrix_t first_t = matrix(fig.widht, fig.hight);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (j < first_t.hight && (fig.hight - i - 1) < first_t.widht &&
          fig.hight - i - 1 >= 0) {
        if (fig.field[i][j] == 1) {
          first_t.field[j][fig.hight - i - 1] = fig.field[i][j];
        }
      }
    }
  }
  return first_t;
}

//// Вращение фигуры ///// Поворот на 90 градусов ///////////////////
matrix_t rotate(player_pos *block_pos, matrix_t fig) {
  matrix_t first_t = matrix(fig.widht, fig.hight);
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (j < first_t.hight && (fig.hight - i - 1) < first_t.widht &&
          fig.hight - i - 1 >= 0) {
        if (block_pos->x + fig.hight == COLS_FIELD + 2) {
          return fig;
        } else {
          first_t.field[j][fig.hight - i - 1] = fig.field[i][j];
        }
      }
    }
  }
  return first_t;
}

/// Cтолкновление ////
bool check_attaching(player_pos block_pos, matrix_t map, matrix_t fig) {
  bool rc = FALSE;
  for (int i = 0; i < fig.hight; i++) {
    for (int j = 0; j < fig.widht; j++) {
      if (map.field[block_pos.y + i][block_pos.x + j] && fig.field[i][j] == 1) {
        rc = TRUE;
      }
    }
  }
  return rc;
}

//////////// Left
player_pos move_left(player_pos *block_pos, matrix_t *map, matrix_t *fig) {
  if (block_pos->x != 1) {
    block_pos->x--;
    if (check_attaching(*block_pos, *map, *fig)) {
      block_pos->x++;
    }
  }
  return *block_pos;
}

//////////// Right
player_pos move_right(player_pos *block_pos, matrix_t *map, matrix_t *fig) {
  if (block_pos->x + fig->widht != COLS_FIELD + 1) {
    block_pos->x++;
    if (check_attaching(*block_pos, *map, *fig)) {
      block_pos->x--;
    }
  }
  return *block_pos;
}

////////// Down
player_pos move_down(player_pos *block_pos, matrix_t *map, matrix_t *fig) {
  if (block_pos->y + fig->hight != ROWS_FIELD + 3) {
    block_pos->y++;
    if (check_attaching(*block_pos, *map, *fig)) {
      block_pos->y--;
    }
  }
  return *block_pos;
}

/// Управление ///

UserAction_t getUserAction(int rc) {
  switch (rc) {
    case ENTER_KEY:
      rc = Start;
      break;
    case SPACE:
      rc = Pause;
      break;
    case ESCAPE:
      rc = Terminate;
      break;
    case KEY_LEFT:
      rc = Left;
      break;
    case KEY_RIGHT:
      rc = Right;
      break;
    case KEY_DOWN:
      rc = Down;
      break;
    case KEY_UP:
      rc = Action;
      break;
  }
  return rc;
}

void userInput(UserAction_t action, bool hold, bool *paused, bool *terminate,
               player_pos *block_pos, matrix_t *map, matrix_t *fig, int h_score,
               int max_score) {
  if (hold) {
    switch (action) {
      case Start:

        break;
      case Pause:
        if (*paused) {
          timeout(-1);
          attron(A_BOLD);
          attron(COLOR_PAIR(4));
          MVPRINTW(19, COLS_FIELD + 23, "%s", "YES");
          attroff(COLOR_PAIR(4));
          attroff(A_BOLD);
          *paused = false;
        } else {
          timeout(0);
          *paused = true;
        }
        break;
      case Terminate:
        if (h_score > max_score) {
          set_score(h_score);
        }
        timeout(-1);
        clear();
        attron(A_BOLD);
        attron(COLOR_PAIR(7));
        MVPRINTW(ROWS_MAP / 2 - 3, (COLS_FIELD) / 2, " GAME_OVER ");
        attroff(COLOR_PAIR(7));
        attroff(A_BOLD);
        print_overlay();
        *terminate = false;
        break;
      case Left:
        if ((*paused != false) && (*terminate != false)) {
          *block_pos = move_left(block_pos, map, fig);
        }
        break;
      case Right:
        if ((*paused != false) && (*terminate != false)) {
          *block_pos = move_right(block_pos, map, fig);
        }
        break;
      case Down:
        if ((*paused != false) && (*terminate != false)) {
          *block_pos = move_down(block_pos, map, fig);
        }
        break;
      case Action:
        if ((*paused != false) && (*terminate != false)) {
          *fig = rotate(block_pos, *fig);
        }
        break;
    }
  }
}
