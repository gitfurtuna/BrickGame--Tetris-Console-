#include "tetris.h"

#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "tet_backend.h"

int main() {
  WIN_INIT(1000000);
  WINDOW* win = initscr();
  nodelay(win, TRUE);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  init_pair(7, COLOR_RED, COLOR_BLACK);

  matrix_t map = field();
  player_pos block_pos;
  matrix_t fig = create_random_block();
  matrix_t next_fig = create_random_block();
  block_pos.y = FIELD_BEGIN, block_pos.x = COLS_FIELD / 2;
  int counter = 0;
  bool paused = true;
  bool terminate = true;
  int h_score = 0;
  int c_score = 0;
  int max_score = 0;
  int speed_0;

  while (getch() != ENTER_KEY) {
    bool show_intro_message = true;
    if (show_intro_message) {
      attron(A_BOLD);
      attron(COLOR_PAIR(3));
      MVPRINTW(ROWS_MAP / 2 - 3, (COLS_FIELD + 11 - INTRO_MESSAGE_LEN) / 2,
               INTRO_MESSAGE);
      attroff(COLOR_PAIR(3));
      attroff(A_BOLD);
    }
    print_overlay();
  }

  while (getch() != ESCAPE) {
    matrix_t save_map = copy_block(map);
    clear();

    usleep(100000);

    add_block(&map, block_pos, fig);
    updateCurrentMap(map);
    level(h_score);
    max_score = get_hight_score();
    attron(A_BOLD);
    attron(COLOR_PAIR(5));
    MVPRINTW(1, COLS_FIELD + 23, "%d", max_score);
    attroff(COLOR_PAIR(5));
    attroff(A_BOLD);

    show_next_block(next_fig);
    attron(A_BOLD);
    attron(COLOR_PAIR(4));
    MVPRINTW(4, COLS_FIELD + 24, "%d", h_score);
    attroff(COLOR_PAIR(4));
    attroff(A_BOLD);

    attron(A_BOLD);
    attron(COLOR_PAIR(4));
    MVPRINTW(19, COLS_FIELD + 23, "%s", "NO");
    attroff(COLOR_PAIR(4));
    attroff(A_BOLD);
    print_overlay();
    refresh();

    int rc = getch();
    UserAction_t action = getUserAction(rc);
    userInput(action, true, &paused, &terminate, &block_pos, &save_map, &fig,
              h_score, max_score);
    counter++;

    speed(level(h_score), speed_0, counter);
    speed_0 = speed(level(h_score), speed_0, counter);
    if (speed_0 == 0)

      if ((block_pos.y + fig.hight < ROWS_FIELD + 3) ||
          !check_attaching(block_pos, save_map, fig)) {
        block_pos.y++;
      }

    if ((block_pos.y == 2 && block_pos.x == COLS_FIELD / 2 &&
         check_attaching(block_pos, save_map, fig))) {
      if (h_score > max_score) {
        set_score(h_score);
      }
      timeout(-1);
      attron(A_BOLD);
      attron(COLOR_PAIR(7));
      MVPRINTW(ROWS_MAP / 2 - 3, (COLS_FIELD) / 2, " GAME_OVER ");
      attroff(COLOR_PAIR(7));
      attroff(A_BOLD);
      print_overlay();
    }

    if ((block_pos.y + fig.hight == ROWS_FIELD + 3) ||
        check_attaching(block_pos, save_map, fig)) {
      c_score = delete_full_line(&map);

      h_score += score(c_score);

      fig = next_fig;
      next_fig = create_random_block();
      block_pos.y = FIELD_BEGIN, block_pos.x = COLS_FIELD / 2;
    } else {
      map = copy_block(save_map);
    }
  }
  endwin();
}
