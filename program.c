#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

typedef struct Bricks {
  int posX, posY;
  int rotation; // (0-3)
  bool data[3][3];
} Brick;

const int height = 15, width = 10, canvasX = 2, canvasY = 2;

const char EXIT_KEY = 'q';
const char EMPTY = ' ';
const char BLOCK = '#';

WINDOW *frame;
WINDOW *win;
Brick currentBrick;

const bool bricks[][3][3] = {
  {
    {true,  true,  false},
    {false, true,  false},
    {false, true,  false}
  },{
    {false, false, false},
    {true,  true,  true},
    {false, false, false}
  }
};

void renderGame();
Brick newBrick();

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
  frame = newwin(height+2,width+2,canvasY,canvasX);
  win = newwin(height,width,canvasY+1,canvasX+1);

  // Init brick
  currentBrick = newBrick();

  // Render frame
  box(frame, 0, 0);
  wrefresh(frame);

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
  // Render current block
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      mvwprintw(
          win,
          currentBrick.posX + x,
          currentBrick.posY + y,
          "%c",
          currentBrick.data[x][y] ? BLOCK : EMPTY
      );
    }
  }
  wrefresh(win);

//  refresh();
}

Brick newBrick() {
  Brick brick;

  brick.posX = 0; // TODO Set to '-3'
  brick.posY = 0;
  brick.rotation = 0;

  // TODO Randomize brick
  int randBrick = 0;

  // Fill data
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      brick.data[x][y] = bricks[randBrick][x][y];
    }
  }

  return brick;
}
