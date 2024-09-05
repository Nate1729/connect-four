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

void print_board(TileState *board, unsigned rows, unsigned columns) {
  unsigned r, c;

  for (r=0; r<rows; r++) {
    for (c=0; c<columns; c++) {
      printf("%c ", tile_state_to_char(board[r*columns + c]));
    }
    printf("\b\n");
  }
}

int main(void) {
  TileState board[ROW_COUNT * COLUMN_COUNT] = {0};
  int quit = 0;
  char input = ' ';
  
  enable_raw_mode();
  do {
    printf("\x1b[2J\x1b[H");
    
    printf("Current input: %c\n\n", input);

    print_board(board, ROW_COUNT, COLUMN_COUNT);
  }
  while (fread(&input, 1, 1, stdin) == 1 && input != 'q');
}
