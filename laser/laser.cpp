#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "laser.h"

using namespace std;

/* pre-supplied helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **array = new char *[rows];
  assert(array);
  for (int r=0; r<rows; r++) {
    array[r] = new char[columns];
    assert(array[r]);
  }
  return array;
}

/* pre-supplied helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **array, int rows) {
  for (int r=0; r<rows; r++)
    delete [] array[r];
  delete [] array;
}

/* internal helper function which gets the dimensions of a board */
bool get_board_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* internal helper function which removes carriage returns and newlines from end of strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **board = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    filter(line);
    strcpy(board[r], line);
  }
  
  return board;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your own function definitions here */

bool find_laser(char** board, int height, int width, int& row){
  for (int r = 0; r < height; r++){
    if (board[r][0] == '>'){
      row = r;
      return true;
    }
  }
  row = -1;
  return false;
}

char mirror_label(char** board, int height, int width, int row, int column){
  // check specified position is on the board
  if (row < 0 || row >= height || column < 0 || column >= width){
    return '\0';
  }
  if (board[row][column] == '/'){
    if (isalpha(board[row - 1][column + 1]) != 0){
      return board[row - 1][column + 1];
    }
    else if (isalpha(board[row + 1][column - 1]) != 0){
      return board[row + 1][column - 1];
    }
  }
  if (board[row][column] == '\\'){
    if (isalpha(board[row - 1][column - 1]) != 0){
      return board[row - 1][column - 1];
    }
    else if (isalpha(board[row + 1][column + 1]) != 0){
      return board[row + 1][column + 1];
    }
  }

  return '\0';
}


/* helper */
void find_laser_absorber(char** board, int height, int width, int& row){
  for (int r = 0; r < height; r++){
    if (board[r][0] == '@'){
      row = r;
      return;
    }
  }
  row = -1;
  return;
}

/* helper */
void direction_to_delta(Direction dir, int& row_delta, int& col_delta){
  switch(dir){
    case NORTH: row_delta = -1;
                col_delta = 0;
                return;
    case SOUTH: row_delta = 1;
                col_delta = 0;
                return;
    case EAST: row_delta = 0;
               col_delta = 1;
               return;
    case WEST: row_delta = 0;
               col_delta = -1;
               return;
    default: row_delta = 0;
             col_delta = 0;
             return;                         
  }
}

/* helper */
void change_direction(Direction& dir, char mirror){
  if (dir == NORTH){
    if (mirror == '\\'){
      dir = WEST;
    }
    else if (mirror == '/'){
      dir = EAST;
    }
  }
  else if (dir == SOUTH){
    if (mirror == '\\'){
      dir = EAST;
    }
    else if (mirror == '/'){
      dir = WEST;
    }
  }
  else if (dir == EAST){
    if (mirror == '\\'){
      dir = SOUTH;
    }
    else if (mirror == '/'){
      dir = NORTH;
    }
  }
  // WEST
  else{
    if (mirror == '\\'){
      dir = NORTH;
    }
    else if (mirror == '/'){
      dir = SOUTH;
    }
  }
  return;
}


/* helper */
void move_laser(char** board, int height, int width, int& row, int& column, Direction& dir){
  int row_delta, col_delta;
  direction_to_delta(dir, row_delta, col_delta);
  row += row_delta;
  column += col_delta;
  //cout << row << " : " << column << endl;

  // we hit a blocking character
  if (board[row][column] == '@' || board[row][column] == '#' || board[row][column] == '?'){
    return;
  }

  // we hit a mirror
  if (board[row][column] == '\\' || board[row][column] == '/'){
    change_direction(dir, board[row][column]);
    return;
  }

  // add laser beam to board
  if (dir == NORTH || dir == SOUTH){
    if (board[row][column] == '-'){
      board[row][column] = '+';
    }
    else{
      board[row][column] = '|';
    }
  }
  else if (dir == EAST || dir == WEST){
    if (board[row][column] == '|'){
      board[row][column] = '+';
    }
    else{
      board[row][column] = '-';
    }
  }
  return;
}


bool shoot(char** board, int height, int width, char* message, int& last_row, int& last_col){
  memset(message, 0, strlen(message));
  int start_row;
  if (!find_laser(board, height, width, start_row)){
    return false;
  }
  
  last_row = start_row;
  last_col = 0;
  // assume we always start by going EAST
  Direction dir = EAST;
  while (board[last_row][last_col] != '@' && board[last_row][last_col] != '#' && board[last_row][last_col] != '?'){

    move_laser(board, height, width, last_row, last_col, dir);
    if (board[last_row][last_col] == '/' || board[last_row][last_col] == '\\'){
      char label = mirror_label(board, height, width, last_row, last_col);
      if (label != '\0'){
        *message = label;
        message++;
      }
    }
    if (board[last_row][last_col] == '@'){
      return true;
    }
  }

  return false;
}

/* helper */
void clear_board(char** board, int height, int width){
  for (int r = 0; r < height; r++){
    for(int c = 0; c < width; c++){
      if (board[r][c] == '|' || board[r][c] == '-' || board[r][c] == '+'){
        board[r][c] = ' ';
      }
    }
  }
}


bool solve(char** board, int height, int width, const char* message){
  // initialise start
  int last_row, last_col;
  char working_message[512] = {};

  if (shoot(board, height, width, working_message, last_row, last_col) && strcmp(message, working_message) == 0){
    return true;
  }
  // we ended on a '?'
  else if (board[last_row][last_col] == '?'){
    char mirror_options[] = {'\\', '/', ' '};
    for (int i = 0; mirror_options[i] != '\0'; i++){
      // set a mirror
      board[last_row][last_col] = mirror_options[i];
      
      // try and solve
      if (solve(board, height, width, message)){
        return true;
      }
      // we could not solve so reset the mirror and clear the board of lasers
      board[last_row][last_col] = '?';
      clear_board(board, height, width);

    }
  }
  return false;
}