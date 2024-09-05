#include <stdio.h>

#include "ui.h"

#define ROW_COUNT 6
#define COLUMN_COUNT 7

typedef enum {
  TILE_STATE_EMPTY,
  TILE_STATE_P1,
  TILE_STATE_P2
} TileState;

char tile_state_to_char(TileState tile) {
  switch (tile) {
    case TILE_STATE_P1:
      return 'X';
    case TILE_STATE_P2:
      return '0';
    case TILE_STATE_EMPTY:
    default:
      return '_';

  }
}

void print_board(TileState *board) {
  unsigned r, c;

  for (r=0; r<ROW_COUNT; r++) {
    for (c=0; c<COLUMN_COUNT; c++) {
      printf("%c ", tile_state_to_char(board[r*COLUMN_COUNT + c]));
    }
    printf("\b\n");
  }
}

typedef enum {
  TURN_P1=0,
  TURN_P2=1,
  QUIT=2
} StateMask;

typedef struct {
  TileState *board;
  StateMask state;
  char user_input;
  unsigned column_hover;
} GameState;

typedef enum {
  LEFT,
  RIGHT
} Direction;

void game_state_move_column_hover(GameState *game_state, Direction dir) {
  if (dir == LEFT && game_state->column_hover > 0)
    game_state->column_hover--;
  else if (dir == RIGHT && game_state->column_hover+1 < COLUMN_COUNT)
    game_state->column_hover++;
  
}

void print_hover_piece(GameState* game_state) {
  char piece;
  if (game_state->state & TURN_P2) 
    piece = tile_state_to_char(TILE_STATE_P2);
  else
    piece = tile_state_to_char(TILE_STATE_P1);

  if (game_state->column_hover) {
    printf("\x1B[%dC%c\n", 2*game_state->column_hover, piece); 
  } else {
    printf("%c\n", piece);
  }
}

void game_loop() {
  TileState board[ROW_COUNT * COLUMN_COUNT] = {0};

  GameState state = {.board=board};

  enable_raw_mode();
  do {
    /* State update */
    if (state.user_input == 'a') {
      game_state_move_column_hover(&state, LEFT);
    } else if (state.user_input == 'd') {
      game_state_move_column_hover(&state, RIGHT);
    }

    printf("\x1b[2J\x1b[H");
    
    printf("Current input: %c\n", state.user_input);
    if (state.state & TURN_P2) {
      printf("It's Player 2's turn!\n");
    } else {
      printf("It's Player 1's turn!\n");
    }
    
    print_hover_piece(&state);
    print_board(board);
  }
  while (fread(&state.user_input, 1, 1, stdin) == 1 && state.user_input != 'q');
}

int main(void) {
  game_loop();
}
