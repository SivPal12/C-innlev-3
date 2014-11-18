#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

const int with = 10, height = 20, canvasX = 2, canvasY = 2;

const char EXIT_KEY = 'q';

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
const int with = 10, height = 20, canvasX = 2, canvasY = 2;
  WINDOW * border = newwin(with+2,height+2,canvasX,canvasY);
  WINDOW * win = newwin(with,height,canvasX+1,canvasY+1);

//  box(border, 0,0);
//  wrefresh(border);
//  refresh();

  box(border, 0, 0);
  wrefresh(border);


  while ((currentKey = getch()) != EXIT_KEY) {
    if (currentKey != ERR) {
      wprintw(win, "%c", currentKey);
    }
    wrefresh(win);
    // ReadUserInput
    // UpdateGameState
    // RenderGame
    refresh();
  }

  endwin();

  return 0;
}
