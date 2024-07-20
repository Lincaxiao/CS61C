#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row,
                         unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static bool is_body(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  // TODO: Implement this function.
  unsigned int row = 18, col = 20;
  unsigned int f_row = 2, f_col = 9;
  unsigned int t_row = 2, t_col = 2;
  unsigned int h_row = 2, h_col = 4;

  game_state_t *game = malloc(sizeof(game_state_t));
  game->num_rows = row;
  game->num_snakes = 1;

  // snake:
  game->snakes = malloc(sizeof(snake_t));
  game->snakes->head_col = h_col;
  game->snakes->head_row = h_row;
  game->snakes->tail_col = t_col;
  game->snakes->tail_row = t_row;
  game->snakes->live = true;

  // board:
  game->board = malloc(sizeof(char *) * row);
  for (int i = 0; i < row; i++) {
    (game->board)[i] = malloc(sizeof(char) * (col + 2));
  }
  memset((game->board)[0], '#', col);
  (game->board)[0][col] = '\n';
  (game->board)[0][col + 1] = '\0';
  strcpy((game->board)[row - 1], (game->board)[0]);

  memset((game->board)[1], ' ', col);
  (game->board)[1][0] = (game->board)[1][col - 1] = '#';
  (game->board)[1][col] = '\n';
  (game->board)[1][col + 1] = '\0';

  for (int i = 2; i < row - 1; i++) {
    strcpy((game->board)[i], (game->board)[1]);
  }

  (game->board)[f_row][f_col] = '*';
  (game->board)[h_row][h_col] = 'D';
  (game->board)[t_row][t_col] = 'd';
  (game->board)[t_row][3] = '>';

  return game;
}

/* Task 2 */
void free_state(game_state_t *state) {
  // TODO: Implement this function.
  if (state) {
    if (state->board) {
      for (int i = 0; i < state->num_rows; i++) {
        free((state->board)[i]);
      }
      free(state->board);
    }
    free(state->snakes);
    free(state);
    state = NULL;
  }

  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  // TODO: Implement this function.
  for (int i = 0; i < state->num_rows; i++) {
    fprintf(fp, "%s", (state->board)[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
    return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x') {
    return true;
  }
  return false;
}

static bool is_body(char c) {
  if (c == '^' || c == '<' || c == 'v' || c == '>') {
    return true;
  }
  return false;
}
/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  if (is_head(c) || is_tail(c) || is_body(c) || c == 'x') {
    return true;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  switch (c)
  {
    case '^':
      return 'w';
    case '<':
      return 'a';
    case 'v':
      return 's';
    case '>':
      return 'd';
  }
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  switch (c)
  {
    case 'W':
      return '^';
    case 'A':
      return '<';
    case 'S':
      return 'v';
    case 'D':
      return '>';
  }
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's' || c == 'S') {
    return cur_row + 1;
  } else if (c == '^' || c == 'w' || c == 'W') {
    return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D') {
    return cur_col + 1;
  } else if (c == '<' || c == 'a' || c == 'A') {
    return cur_col - 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int row = state->snakes[snum].head_row;
  unsigned int col = state->snakes[snum].head_col;
  char c = get_board_at(state, get_next_row(row, state->board[row][col]), get_next_col(col, state->board[row][col]));
  return c;
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  char head = state->board[state->snakes[snum].head_row][state->snakes[snum].head_col];
  unsigned int next_row = get_next_row(state->snakes[snum].head_row, head);
  unsigned int next_col = get_next_col(state->snakes[snum].head_col, head);
  // set new head, ignore food, walls, and snake bodies
  set_board_at(state, next_row, next_col, head);
  // change old head to body
  set_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col, head_to_body(head));
  // update head row and col
  state->snakes[snum].head_row = next_row;
  state->snakes[snum].head_col = next_col;
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  char tail = state->board[state->snakes[snum].tail_row][state->snakes[snum].tail_col];
  unsigned int next_row = get_next_row(state->snakes[snum].tail_row, tail);
  unsigned int next_col = get_next_col(state->snakes[snum].tail_col, tail);
  char body = get_board_at(state, next_row, next_col);
  char new_tile = body_to_tail(body);
  // set new tail
  set_board_at(state, next_row, next_col, new_tile);
  // change old tail to blank
  set_board_at(state, state->snakes[snum].tail_row, state->snakes[snum].tail_col, ' ');
  // update tail row and col
  state->snakes[snum].tail_row = next_row;
  state->snakes[snum].tail_col = next_col;
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  for (unsigned int i = 0; i < state->num_snakes; i++) {
    if (!state->snakes[i].live) {
      continue;
    }
    char next = next_square(state, i);
    if (next == '*') {
      add_food(state);
      update_head(state, i);
      continue;
    } else if (is_snake(next) || next == '#') {
      state->snakes[i].live = false;
      // change the head to 'x'
      set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
      continue;
    } else {
      update_head(state, i);
      update_tail(state, i);
    }
  }
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
    size_t buffer_size = 128;
    size_t line_len = 0;
    char *line = NULL;
    char *buffer = malloc(buffer_size * sizeof(char));
    
    if (buffer == NULL) {
        return NULL;
    }

    while (fgets(buffer, (int) buffer_size, fp)) {
        size_t buffer_len = strlen(buffer);
        line_len += buffer_len;

        // Reallocate memory for line to hold the new data
        char *new_line = realloc(line, sizeof(char) * (line_len + 1));
        if (new_line == NULL) {
            free(line);  // Free previously allocated memory
            free(buffer);  // Free buffer
            return NULL;
        }
        line = new_line;

        // Copy or concatenate buffer to line
        if (line_len == buffer_len) {
            strcpy(line, buffer);
        } else {
            strcat(line, buffer);
        }

        // If the buffer ends with a newline, return the line
        if (buffer[buffer_len - 1] == '\n') {
            free(buffer);
            return line;
        }
    }

    free(buffer);
    return line;  // Return the line, which may be NULL if nothing was read
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  game_state_t *game = malloc(sizeof(game_state_t));
  if (game == NULL) {
    return NULL;
  }
  unsigned int row = 100;
  game->board = malloc(sizeof(char *) * row);
  if (game->board == NULL) {
    free(game);
    return NULL;
  }
  unsigned int real_rows = 0;
  for (char* line = read_line(fp); line != NULL; real_rows++) {
    if (real_rows >= row) {
      row *= 2;
      game->board = realloc(game->board, sizeof(char *) * row);
    }
    (game->board)[real_rows] = line;
    line = read_line(fp);
  }
  if (0 == real_rows) {
    return NULL;
  } else {
    game->board = realloc(game->board, sizeof(char *) * real_rows);
    game->num_rows = real_rows;
    game->num_snakes = 0;
    game->snakes = NULL;
    return game;
  }
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int tailRow = state->snakes[snum].tail_row;
  unsigned int tailCol = state->snakes[snum].tail_col;
  unsigned int headRow = tailRow;
  unsigned int headCol = tailCol;
  char head = state->board[tailRow][tailCol];
  while (is_body(head) || is_tail(head)) {
    headRow = get_next_row(headRow, head);
    headCol = get_next_col(headCol, head);
    head = state->board[headRow][headCol];
  }
  state->snakes[snum].head_row = headRow;
  state->snakes[snum].head_col = headCol;
  if (head == 'x') {
    state->snakes[snum].live = false;
  } else {
    state->snakes[snum].live = true;
  }
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  unsigned int numberofSnakes = 0;
  for (unsigned int i = 0; i < state->num_rows; i++) {
    for (unsigned int j = 0; j < strlen(state->board[i]); j++) {
      if (is_tail(state->board[i][j])) {
        numberofSnakes++;
        state->snakes = realloc(state->snakes, sizeof(snake_t) * numberofSnakes);
        if (state->snakes == NULL) {
          return NULL;
        }
        state->snakes[numberofSnakes - 1].tail_row = i;
        state->snakes[numberofSnakes - 1].tail_col = j;
        find_head(state, numberofSnakes - 1);
      }
    }
  }
  state->num_snakes = numberofSnakes;
  return state;
}
