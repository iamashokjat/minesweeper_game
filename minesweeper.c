#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define HORIZONTALLINE 196
#define DHORIZONTALLINE 205
#define VERTICALLINE 179

#define BEGINNER 1
#define INTERMEDIATE 2
#define ADVANCED 3 

#define BOX 254
#define ASTERISK 42
#define FLAG 70 

#define true 1
#define false 0

int side; //to store the side of the minesweeper
int bombs_count; //to store the bombs count
int flags_left; //denote the number of flags left
int moves_left; //denotes the number of moves left

void select_difficulty();
void game_title();
void print_board(int ** , int ** );
void place_bombs(int ** , int ** );
void increment_surroundings(int ** , int, int);
int is_valid(int, int);
int is_bomb(int ** , int, int);
int apply_movement(int ** , int ** , int ** , int, int, int * );

int main() {

  int i, j;
	
  game_title(); // first screen of the game

  select_difficulty(); // second screen of the game 

	//Dynamically allocating memory for Boards
  int ** Board = (int ** ) calloc(side, sizeof(int * ));
  for (i = 0; i < side; i++) {
    Board[i] = (int * ) calloc(side, sizeof(int));
  }

  int ** DisplayBoard = (int ** ) calloc(side, sizeof(int * ));
  for (i = 0; i < side; i++) {
    DisplayBoard[i] = (int * ) calloc(side, sizeof(int));
  }

  int ** Bombs = (int ** ) malloc(bombs_count * sizeof(int * ));
  for (i = 0; i < bombs_count; i++) {
    Bombs[i] = (int * ) malloc(2 * sizeof(int));
  }

  //placing the bombs @ random positions
  place_bombs(Board, Bombs);

  int is_game_over = false;
  int row, col;

  moves_left = side * side - bombs_count;

  print_board(Board, DisplayBoard);

  while (is_game_over == false)  // We'll continue till the game is not over
  {
    print_board(Board, DisplayBoard);

    printf("Press M or F:\n\t\t\t");
    char input = getch();

    switch (input) {
    
    //F to flag/unflag a location
    case 'f':
    case 'F':
      printf("Enter row,col to Flag/UnFlag: ");
      scanf("%d%d", & row, & col);
      row -= 1;
      col -= 1;
      if (DisplayBoard[row][col] == -2) {
        DisplayBoard[row][col] = 0;
        flags_left++;
      } else if (DisplayBoard[row][col] == 0) {
        DisplayBoard[row][col] = -2;
        flags_left--;
      }
      print_board(Board, DisplayBoard);
      break;
    
    //to open a location
    case 'm':
    case 'M':
      printf("Enter the row,col to open: ");
      scanf("%d%d", & row, & col);
      row -= 1;
      col -= 1;
      break;
    }

    if (input == 'f' || input == 'F') {
      continue;
    }

    is_game_over = apply_movement(Board, DisplayBoard, Bombs, row, col, & moves_left); // opening the user's provided locationon board and checking whether the game is over

    if ((is_game_over == false) && (moves_left == 0)) { 
    printf("\n\n\n\t\t\t\t\t");
    
    for (i = 0; i < 25; i++) {
      printf("*");
    }
    printf("\n\t\t\t\t\t\tYOU WON :\n");

    printf("\t\t\t\t\t");
    for (i = 0; i < 25; i++) {
      printf("*");
    }
      is_game_over = true;
    }
  }

  return 0;
}

//function to display the Game title card
void game_title() {
  int i;
  for (i = 0; i < 10; i++) {
    printf("\n");
  }
  printf("\t\t\t\t");

  for (i = 0; i < 50; i++) {
    printf("*");
  }

  printf("\n\n\t\t\t\t\t\t");

  char title[] = "*** MINE-SWEEPER ***";
  int len = strlen(title);
  for (i = 0; i < len; i++) {
    printf("%c", title[i]);
    usleep(250000);
  }
  usleep(250000);
  printf("\n\n\t\t\t\t");

  for (i = 0; i < 50; i++) {
    printf("*");
  }
  usleep(2000000);
  system("cls");
}


//function to select the difficulty
void select_difficulty() {

  int difficulty, i;
  system("cls");
  printf("\t\t\t");

  for (i = 0; i < 50; i++) {
    printf("%c", DHORIZONTALLINE);
  }

  printf("\n\n\t\t\t\t\tSELECT DIFFICULTY\n\n");

  printf("\t\t\t");
  for (i = 0; i < 50; i++) {
    printf("%c", DHORIZONTALLINE);
  }
  printf("\n\n\t\t\t   1. BEGINNER (9 * 9 Cells and 10 Mines)\n\n");
  printf("\t\t\t   2. INTERMEDIATE (16 * 16 Cells and 40 Mines)\n\n");
  printf("\t\t\t   3. ADVANCED (24 * 24 Cells and 99 Mines\n\n");
  printf("\n\t\t\tEnter option: ");
  scanf("%d", & difficulty);

  while (1) {
    if (difficulty >= 1 && difficulty <= 3) {
      break;
    } else {
      printf("\a");
      select_difficulty();
    }
  }

  if (difficulty == BEGINNER) {
    side = 9;
    bombs_count = 10;
    flags_left = 10;
  }

  if (difficulty == INTERMEDIATE) {
    side = 16;
    bombs_count = 40;
    flags_left = 40;
  }

  if (difficulty == ADVANCED) {
    side = 24;
    bombs_count = 99;
    flags_left = 99;
  }

  return;
}


//function to print the board 
void print_board(int ** Board, int ** DisplayBoard) {

  system("cls");
  int i, j, k;

  printf("\t\t\t");

  for (i = 0; i < 60; i++) {
    printf("%c", DHORIZONTALLINE);
  }

  printf("\n\n\t\t\t\t\t\tMINE-SWEEPER\n\n");

  printf("\t\t\t");
  for (i = 0; i < 60; i++) {
    printf("%c", DHORIZONTALLINE);
  }

  printf("\n\t\t\t FLAGS LEFT:%2d", flags_left);

  printf("\t\t\tPRESS F for FLAG/UNFLAG");
  printf("\n\t\t\t MOVES LEFT:%2d", moves_left);
  printf("\t\t\tPRESS M for Move");

  printf("\n\n\t\t\t   ");

  for (i = 1; i <= side; i++)
    printf("%3d ", i);

  printf("\n\n\t\t\t   ");

  for (k = 0; k < side; k++) {
    printf(" ");
    printf("%2c", HORIZONTALLINE);
    printf(" ");
  }

  printf("\n\t\t\t");

  for (i = 0; i < side; i++) {
    printf("%2d ", i + 1);
    printf("%c", VERTICALLINE);
    for (j = 0; j < side; j++) {
      if (DisplayBoard[i][j] > 0) {
        if (Board[i][j] == 0) {
          printf("   ");
        } else {
          printf("%2d ", Board[i][j]);
        }
      } else if (DisplayBoard[i][j] == -2) {
        printf("%2c ", FLAG);
      } else if (DisplayBoard[i][j] == -1) {
        printf("%2c ", ASTERISK);
      } else {
        printf("%2c ", BOX);
      }

      if (j != side - 1) {
        printf("%c", VERTICALLINE);
      }
    }

    printf("%c\n\t\t\t   ", VERTICALLINE);
    for (k = 0; k < side; k++) {
      printf(" ");
      printf("%2c", HORIZONTALLINE);
      printf(" ");
    }

    printf("\n\t\t\t");
  }
  return;
}


//function to place the bomb at random location
void place_bombs(int ** Board, int ** Bombs) {
  int visited[side * side];
  int i, k, l;

  memset(visited, false, sizeof(visited));

  srand(time(0));

  for (i = 0; i < bombs_count; i++) 
  {
    int random = rand() % (side * side);
    int x = random / side;
    int y = random % side;

    if (visited[random] == false) {
      Bombs[i][0] = x;
      Bombs[i][1] = y;
      Board[Bombs[i][0]][Bombs[i][1]] = -1;
      visited[random] = true;
    } 
	else {
      int flag = 0;
      for (k = 0; k < side; i++) {
        for (l = 0; l < side; l++) {
          if (Board[k][l] == 0) {
            Bombs[i][0] = x;
            Bombs[i][1] = y;
            Board[Bombs[i][0]][Bombs[i][1]] = -1;
            visited[random] = true;
            flag = 1;
          }
        }
        if (flag == 1) {
          break;
        }
      }
    }

    increment_surroundings(Board, x, y);
  }
}


//Function to check whether the given location is within the boundary
int is_valid(int row, int col) {
  return ((row >= 0) && (row < side) && (col >= 0) && (col < side));
}


//function to check whether the given row and column is Bomb or not
int is_bomb(int ** Board, int row, int col) {
  if (Board[row][col] == -1)
    return true;
  else
    return false;
}


//function to increment the surrounding after the bomb is placed
void increment_surroundings(int ** Board, int row, int col) {
  if (is_valid(row - 1, col) == true) {         // Top
    if (is_bomb(Board, row - 1, col) == false) {
      Board[row - 1][col]++;
    }
  }

  if (is_valid(row + 1, col) == true) {    // Bottom
    if (is_bomb(Board, row + 1, col) == false) {
      Board[row + 1][col]++;
    }
  }

  if (is_valid(row, col + 1) == true) {       // Right
    if (is_bomb(Board, row, col + 1) == false) {
      Board[row][col + 1]++;
    }
  }

  if (is_valid(row, col - 1) == true) {		  // Left
    if (is_bomb(Board, row, col - 1) == false) {
      Board[row][col - 1]++;
    }
  }

  if (is_valid(row - 1, col + 1) == true) {    // Top Right
    if (is_bomb(Board, row - 1, col + 1) == false) {
      Board[row - 1][col + 1]++;
    }
  }

  if (is_valid(row - 1, col - 1) == true) {       // Top left
    if (is_bomb(Board, row - 1, col - 1) == false) {
      Board[row - 1][col - 1]++;
    }
  }

  if (is_valid(row + 1, col + 1) == true) {     // Bottom Right
    if (is_bomb(Board, row + 1, col + 1) == false) {
      Board[row + 1][col + 1]++;
    }
  }

  if (is_valid(row + 1, col - 1) == true) {  // Bottom Left
    if (is_bomb(Board, row + 1, col - 1) == false) {
      Board[row + 1][col - 1]++;
    }
  }

  return;
}

int apply_movement(int ** Board, int ** DisplayBoard, int ** Bombs, int row, int col, int * moves_left) {

  // Base Case of Recursion 
  if (DisplayBoard[row][col] != 0)
    return false;

  int i, j;

  if (Board[row][col] == -1) // If user has opened a bomb
  {
    DisplayBoard[row][col] = -1;

    for (i = 0; i < bombs_count; i++)
      DisplayBoard[Bombs[i][0]][Bombs[i][1]] = -1;

    print_board(Board, DisplayBoard);

    printf("\n\n\n\t\t\t\t\t");
    for (i = 0; i < 25; i++) {
      printf("*");
    }
    printf("\n\t\t\t\t\t\tYOU LOST\n");

    printf("\t\t\t\t\t");
    for (i = 0; i < 25; i++) {
      printf("*");
    }

    return true;
  } 
  else 
  {
     
	(*moves_left)--;
    
    if (Board[row][col] == 0) { // If it is an empty location recur in eight directions
    	
      if (is_valid(row - 1, col) == true) {   		// Top
        if (is_bomb(Board, row - 1, col) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row - 1, col, moves_left);
        }
      }

      if (is_valid(row + 1, col) == true) {         //Bottom
        if (is_bomb(Board, row + 1, col) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row + 1, col, moves_left);
        }

      }

      if (is_valid(row, col + 1) == true) {       //Right
        if (is_bomb(Board, row, col + 1) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row, col + 1, moves_left);
        }
      }

      if (is_valid(row, col - 1) == true) {     //Left
        if (is_bomb(Board, row, col - 1) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row, col - 1, moves_left);
        }
      }

      if (is_valid(row - 1, col + 1) == true) {  //TOp Right
        if (is_bomb(Board, row - 1, col + 1) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row - 1, col + 1, moves_left);
        }
      }

      if (is_valid(row - 1, col - 1) == true) {  //Top Left
        if (is_bomb(Board, row - 1, col - 1) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row - 1, col - 1, moves_left);
        }
      }

      if (is_valid(row + 1, col + 1) == true) { //Bottom Right
        if (is_bomb(Board, row + 1, col + 1) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row + 1, col + 1, moves_left);
        }
      }

      if (is_valid(row + 1, col - 1) == true) { //Bottom Left
        if (is_bomb(Board, row + 1, col - 1) == false) {
          DisplayBoard[row][col] = 1;
          apply_movement(Board, DisplayBoard, Bombs, row + 1, col - 1, moves_left);
        }
      }
    } 
	else if (Board[row][col] >= 1) { 
      DisplayBoard[row][col] = 1;
    }

    return false;
  }
}