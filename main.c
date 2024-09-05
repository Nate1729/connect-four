#include <stdio.h>

#include "ui.h"

#define ROW_COUNT 6
#define COLUMN_COUNT 7
#define DEBUG 1

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
  QUIT=2,
  WIN_P1=4,
  WIN_P2=8
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

void game_state_change_turn(GameState *game_state) {
  game_state->state ^= 1;
}



int game_state_check_for_win(GameState *game_state) {
  unsigned r, c;

  TileState player_to_check, *val;
  if (game_state->state & TURN_P2)
    player_to_check = TILE_STATE_P2;
  else
    player_to_check = TILE_STATE_P1;

  for (r=0; r<ROW_COUNT; r++) {
    for (c=0; c<COLUMN_COUNT; c++) {
      val = &game_state->board[r*COLUMN_COUNT + c];

      if (c < COLUMN_COUNT-4) {
        /* Look for wins to the right*/
        if (*val & *(val+1) & *(val+2) & *(val+3)) {
          return 1;
        }
      }  

      if (r < ROW_COUNT-4) {
        /* Look for values down */
        if (*val & *(val+COLUMN_COUNT) & *(val+2*COLUMN_COUNT) & *(val+3*COLUMN_COUNT)) {
          return 1;
        }
      }

      if (c < COLUMN_COUNT-4 && r < ROW_COUNT-4) {
        if (*val & *(val+COLUMN_COUNT+1) & *(val+2*COLUMN_COUNT+2) & *(val+3*COLUMN_COUNT+3)) {
          return 1;
        }
      }
      if (c >= 3 && r < ROW_COUNT-4) {
        /* Check for win going
         *   /
         *  /
         * /
         */
        if (*val & *(val+COLUMN_COUNT-1) & *(val+2*(COLUMN_COUNT-1)) & *(val+3*(COLUMN_COUNT-1))) {
          return 1;
        }
      }
         
    }
  }

  return 0;
  
}

void game_state_drop_piece(GameState *game_state) {
  TileState tile;
  TileState *val;
  unsigned r;
  if (game_state->state & TURN_P2) 
    tile = TILE_STATE_P2;
  else
    tile = TILE_STATE_P1;

  for (r=ROW_COUNT-1; r>=0; r--) {
    val = &game_state->board[r*COLUMN_COUNT + game_state->column_hover];
    if (*val == TILE_STATE_EMPTY) {
      *val = tile; 
      if (game_state_check_for_win(game_state)) {
        if (tile == TILE_STATE_P1) {
          game_state->state |= WIN_P1;
          return;
        } else {
          game_state->state |= WIN_P2;
          return;
        }
      }
      game_state_change_turn(game_state);
      return;
    }
  }
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
    } else if (state.user_input == '\n') {
      game_state_drop_piece(&state);
    }



    printf("\x1b[2J\x1b[H");
    
    if (DEBUG) {
      printf("Current input: %c\n", state.user_input);
      printf("Player 2 Won: %d\n", state.state & WIN_P2);
      printf("Player 1 Won: %d\n", state.state & WIN_P1);
      printf("Both players: %d\n", state.state & (WIN_P1 | WIN_P2));
    }
    if (state.state & TURN_P2) {
      printf("It's Player 2's turn!\n");
    } else {
      printf("It's Player 1's turn!\n");
    }
    
    print_hover_piece(&state);
    print_board(board);
    if (state.state & (WIN_P2 | WIN_P2)) {
      printf("I should break\n");
      break;
    }
  }
  while (fread(&state.user_input, 1, 1, stdin) == 1 && state.user_input != 'q');
  if (state.state & WIN_P2) {
    printf("\nPlayer 2 Wins!\n");
  } else if (state.state & WIN_P1) {
    printf("\nPlayer 1 Wins!\n");
  }

}

int main(void) {
  game_loop();
}
