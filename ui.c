#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "ui.h"

struct termios orig_termios;

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
  struct termios raw; 
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

