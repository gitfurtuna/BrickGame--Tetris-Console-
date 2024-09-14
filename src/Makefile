all: clean clang out tetris tetris_tsmtable
CC := gcc
CFLAGS := -I inc -std=c11 -Wall -Werror -Wextra -Wpedantic
OBJS_FSMSWITCHCASE := out/tetris.o out/backend.o out/frontend.o out/tsm.o 
OBJS_FSMTABLE := out/tetris.o out/backend.o out/frontend.o out/tsm.o 

tetris:  $(OBJS_FSMSWITCHCASE)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

tetris_tsmtable: $(OBJS_FSMTABLE)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) out/*.o tetris tetris_tsmtable

clang:
	clang-format -i --style=google ./src/*.c ./inc/*.h 
	cp ../materials/linters/.clang-format ../src/.clang-format;
	clang-format -style=Google -n ./src/*.c ./inc/*.h 
	rm .clang-format
