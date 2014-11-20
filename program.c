#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

typedef struct Bricks {
  int posX, posY;
  bool data[3][3];
} Brick;

const int canvasX = 2, canvasY = 2;

#define height 20
#define width 10

const char EXIT_KEY = 'q';
const char EMPTY = ' ';
const char BLOCK = '#';

WINDOW *frame;
WINDOW *win;
WINDOW *winScore;
Brick currentBrick;
bool staticBricks[width][height];

typedef struct {
  int score;
  int posX;
  int posY;
  int boardWidth;
  int boardHeight;
} ScoreBoard;
ScoreBoard scoreBoard;

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
void moveBrick(int key);
void rotateCurrBrick();
void moveBrickLeft();
void moveBrickRight();


int main (int argc, char *argv[]) {
  // Setup screen
  initscr();

  // Add keypad
  keypad(stdscr, TRUE);

  // No echo
  noecho();

  // No cursor
  curs_set(0);

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

  // Init score
  scoreBoard.score = 0;
  scoreBoard.posX = canvasX + width + 2;
  scoreBoard.posY = canvasY;
  scoreBoard.boardWidth = 30;
  scoreBoard.boardHeight = 3;
  winScore = newwin(
      scoreBoard.boardHeight,
      scoreBoard.boardWidth,
      scoreBoard.posY,
      scoreBoard.posX);
  box(winScore, 0,0);
  mvwprintw(winScore,1,1, "Score: %d", scoreBoard.score);
  wrefresh(winScore);

  // Game loop
  while ((currentKey = getch()) != EXIT_KEY) {
    // ReadUserInput
    if (currentKey != ERR) {
      // Modify brick
      moveBrick(currentKey);

    } else {
      // UpdateGameState
      updateGameState();
    }
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

  //Render statics
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (staticBricks[x][y]) {
        mvwprintw(win, y, x, "X");
      }
    }
  }
  wrefresh(win);

  // Render scores
  mvwprintw(winScore,1,8, "%d", scoreBoard.score);
  wrefresh(winScore);


//  refresh();
}

Brick newBrick() {
  Brick brick;

  brick.posX = 0; // TODO Set to '-3'
  brick.posY = 0;

  // TODO Randomize brick
  int randBrick = 1;

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
        for (int line = row; line > 0; line --) {
          for (int block = 0; block < width; block++) {
            staticBricks[block][line] = staticBricks[block][line-1];
          }
        }
        // Blank first line
        for (int i = 0; i < width; i++) {
          staticBricks[0][i] = false;
        }

        // Increment score
        scoreBoard.score++;

        // Check next line
        row++;
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
    // Hackish. I know
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

void moveBrick(int key) {
  switch (key) {
    case KEY_UP:
      rotateCurrBrick();
      break;
    case KEY_RIGHT:
      moveBrickRight();
      break;
    case KEY_LEFT:
      moveBrickLeft();
      break;
    default:
    case ERR:
      break;
  }
}

void rotateCurrBrick(){
  bool tmpBrick[3][3];
  memcpy(tmpBrick, currentBrick.data, sizeof(currentBrick.data));
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      currentBrick.data[i][j] = tmpBrick[2-j][i];
    }
  }
}

void moveBrickLeft() {
  for (int col = 0; col < 3; col++) {
    for (int row = 0; row < 3; row++) {
      // If current brick has block
      if (currentBrick.data[col][row]) {
        // Check if game border is NOT to the left
        if (currentBrick.posX + col > 0) {
          // Check if a static brick is to the left
          if (!staticBricks
              [currentBrick.posX + col - 1]
              [currentBrick.posY + row]) {
            currentBrick.posX--;
          }
        }
        return;
      }
    }
  }
}

void moveBrickRight(){
  for (int col = 2; col >= 0; col--) {
    for (int row = 0; row < 3; row++) {
      // If current brick has block
      if (currentBrick.data[col][row]) {
        // Check if game border is NOT to the right
        if (currentBrick.posX + col + 1 < width) {
          // Check if a static brick is to the right
          if (!staticBricks
              [currentBrick.posX + col + 1]
              [currentBrick.posY + row]) {
            currentBrick.posX++;
          }
        }
        return;
      }
    }
  }
}
