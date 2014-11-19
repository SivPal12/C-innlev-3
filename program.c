#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>

typedef struct Bricks {
  int posX, posY;
  bool data[3][3];
} Brick;

const int canvasX = 2, canvasY = 2;

#define height 15
#define width 10

const char EXIT_KEY = 'q';
const char EMPTY = ' ';
const char BLOCK = '#';

WINDOW *frame;
WINDOW *win;
WINDOW *winStatics;
Brick currentBrick;
bool staticBricks[width][height];
int score = 0;

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
void updateGameState();
void updateGameState();
void moveCurrentBrickDown();
void checkForCompleteRows();
bool currentBrickTouchesStatics();
void addCurrentBrickToStatics();


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
  winStatics = newwin(height,width,canvasY+1,canvasX+1);

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
    updateGameState();
    // RenderGame
    renderGame();
  }

  endwin();

  return 0;
}

void renderGame() {
  // Render current block
  werase(win);
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      mvwprintw(
          win,
          currentBrick.posY + y,
          currentBrick.posX + x,
          "%c",
          currentBrick.data[x][y] ? BLOCK : EMPTY
      );
    }
  }
  wrefresh(win);

  //Render statics
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (staticBricks[x][y]) {
        mvwprintw(winStatics, y, x, "X");
      }
    }
  }
  wrefresh(winStatics);

//  refresh();
}

Brick newBrick() {
  Brick brick;

  brick.posX = 0; // TODO Set to '-3'
  brick.posY = 0;

  // TODO Randomize brick
  int randBrick = 0;

  // Fill data
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      // Rotate brick
      brick.data[x][y] = bricks[randBrick][y][x];
    }
  }

  return brick;
}

void updateGameState() {
  moveCurrentBrickDown();
  checkForCompleteRows();
}

void moveCurrentBrickDown() {
  if (currentBrickTouchesStatics()) {
    addCurrentBrickToStatics();
  } else {
    currentBrick.posY++;
  }
}

void checkForCompleteRows() {
  for (int row = height-1; row >= 0; row--) {
    for (int col = 0; col < width; col++) {
      // If line got empty space, got to next
      if (!staticBricks[col][row]) {
        break;
      }
      // If all are filled, move above lines down,
      // increment score and check next line
      if (col >= width-1) {
        //Move lines down
        for (int line = row - 1; line > 0; line--) {
          for (int field = 0; field < width; field++) {
            staticBricks[col][row] = staticBricks[col][line];
          }
        }
        // Blank first line
        for (int i = 0; i < width; i++) {
          staticBricks[i][0] = false;
        }

        // Increment score
        score++;

        // Check next line
        row--;
      }
    }
  }
}

// Or bottom
bool currentBrickTouchesStatics(){
  int brickRowNum = -1;
  bool brickRow[3];

  // For each line in current brick
  for (int row = 2; row >= 0; row--) {
    // For each block
    for (int col = 0; col < 3; col++) {
      // If current brick has a block on this line
      if (currentBrick.data[col][row]) {
        // Save row number
        brickRowNum = row;
        for (int i = 0; i < 3; i++) {
          brickRow[i] = currentBrick.data[i][brickRowNum];
        }
        break;
      }
    }
    if (brickRowNum != -1) {
      break;
    }
  }

  // If last line
  if (currentBrick.posY + brickRowNum + 1 >= height) {
    return true;
  }

  // For each column in current bricks lowest non-empty line
  for (int col = 0; col < 3; col++) {
    // If brick has block on this column
    if (brickRow[col]) {
      // Check if below is static block
      if (staticBricks
          [currentBrick.posX + col]
          [currentBrick.posY + brickRowNum + 1]) {
        return true;
      }
    }
  }
  return false;
}

// Adds current brick to statics and inits new brick
void addCurrentBrickToStatics() {
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      // If staticBricks' block is empty, add currentBricks' block
      if (!staticBricks
          [currentBrick.posX + col]
          [currentBrick.posY + row]) {
        staticBricks
            [currentBrick.posX + col]
            [currentBrick.posY + row]
            = currentBrick.data[col][row];
      }
    }
  }
  currentBrick = newBrick();
}
