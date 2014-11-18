#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

const int height = 15, width = 10, canvasX = 2, canvasY = 2;

const char EXIT_KEY = 'q';
const char EMPTY = ' ';
const char BLOCK = '#';

WINDOW *frame;
WINDOW *win;


const bool bricks[][3][3] = {
  {
    {false, true,  false},
    {false, true,  false},
    {false, true,  false}
  },{
    {false, false, false},
    {true,  true,  true},
    {false, false, false}
  }
};

void renderGame();

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
  frame = newwin(height+2,width+2,canvasX,canvasY);
  win = newwin(height,width,canvasX+1,canvasY+1);

  // Game loop
  while ((currentKey = getch()) != EXIT_KEY) {
    // ReadUserInput
    // Modify brick


    // UpdateGameState
    // RenderGame
    renderGame();
  }

  endwin();

  return 0;
}


void renderGame() {
  wrefresh(win);

  box(frame, 0, 0);
  wrefresh(frame);
  refresh();
}
