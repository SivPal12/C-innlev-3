#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

const int with = 10, height = 20, canvasX = 2, canvasY = 2;

const char EXIT_KEY = 'q';
const char EMPTY = ' ';
const char BLOCK = 219;

const bool bricks[][3][3] = {
  {
    {false, true, true},
    {false, true, true},
    {false, true, true}
  }
};

int main (int argc, char *argv[]) {
  // Setup screen
  initscr();

  // Add keypad
  keypad(stdscr, TRUE);

  // No echo
  noecho();

  // Disable line buffer
  cbreak();

  // input listening mode
  halfdelay(1);

  // Start program
  int currentKey = getch();

  // init window
  WINDOW * border = newwin(with+2,height+2,canvasX,canvasY);
  WINDOW * win = newwin(with,height,canvasX+1,canvasY+1);

  // Draw frame
  box(border, 0, 0);
  wrefresh(border);

  // ReadUserInput
  while ((currentKey = getch()) != EXIT_KEY) {
    if (currentKey != ERR) {
      wprintw(win, "%c", currentKey);
    }
    // UpdateGameState
    // RenderGame
    wrefresh(win);
    refresh();
  }

  endwin();

  return 0;
}
