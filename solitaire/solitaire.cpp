#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "solitaire.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a board */
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

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  cout << "Loading puzzle board from '" << filename << "'...";
  
  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }

  cout << " done (height = " << height << ", width = " << width << ")." << endl;
    
  return m;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << static_cast<char>('A' + c);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your function definitions below */
bool are_identical(char** first, char** second, int height, int width){
  for (int row = 0; row < height; row++){
    for (int col = 0; col < width; col++){
      if (first[row][col] != second[row][col]){
        return false;
      }
    }
  }
  return true;
}

/* helper */
int pos_to_col(const char* move){
  return move[0] - 65;
}

/* helper */
int pos_to_row(const char* move){
  return move[1] - 48;
}

/* helper */
void set_move_delta(const char* move, int& row_delta, int& col_delta){
  switch(move[2]){
    case 'N': row_delta = -2;
              col_delta = 0;
              return;
    case 'S': row_delta = 2;
              col_delta = 0;
              return;
    case 'E': row_delta = 0;
              col_delta = 2;
              return;
    case 'W': row_delta = 0;
              col_delta = -2;
              return;
    default: cerr << "Invalid move!" << endl;
             row_delta = 0;
             col_delta = 0;
             return;
  }
}


bool make_move(char** board, const char* move, int height, int width){
  // check move is of valid length
  if (strlen(move) != 3){
    return false;
  }
  // check start position is somewhere on the board
  int start_row, start_col;
  start_row = pos_to_row(move);
  start_col = pos_to_col(move);
  if (start_row < 0 || start_row >= height || start_col < 0 || start_col >= width ){
    return false;
  }

  // check start position is a marble
  if (board[start_row][start_col] != 'O'){
    return false;
  }

  // check end position is on the board
  int row_delta = 0;
  int col_delta = 0;
  set_move_delta(move, row_delta, col_delta);

  if (start_row + row_delta < 0 || start_row + row_delta >= height || start_col + col_delta < 0 || start_col + col_delta >= width ){
    return false;
  }

  // check end position is empty
  if (board[start_row + row_delta][start_col + col_delta] != '_'){
    return false;
  }

  // check we can hop over something
  if (board[start_row + row_delta/2][start_col + col_delta/2] != 'O'){
    return false;
  }

  // try and make the move
  // make start location empty
  board[start_row][start_col] = '_';
  // assign target spot to a marble
  board[start_row + row_delta][start_col + col_delta] = 'O';
  // remove marble that was hopped over
  board[start_row + row_delta/2][start_col + col_delta/2] = '_';

  return true;
}


/* helper */
char col_to_pos(int col){
  return col + 65;
}

/* helper */
char row_to_pos(int row){
  return row + 48;
}

/* helper */
void generate_move(char* move, int row, int col, char direction){
  move[0] = col_to_pos(col);
  move[1] = row_to_pos(row);
  move[2] = direction;
  move[3] = '\0';
}

/* helper */
void unmake_move(char** board, const char* move){
  int start_row, start_col;
  start_row = pos_to_row(move);
  start_col = pos_to_col(move);
  int row_delta = 0;
  int col_delta = 0;
  set_move_delta(move, row_delta, col_delta);
  board[start_row][start_col] = 'O';
  board[start_row + row_delta][start_col + col_delta] = '_';
  board[start_row + row_delta/2][start_col + col_delta/2] = 'O';
}


bool find_solution_recursive_helper(char** begin, char** end, int height, int width, char* solution, int depth = 0){
  // base case: we are at the solution
  if (are_identical(begin, end, height, width)){
    // remove final comma
    *--solution = '\0';
    return true;
  }
  // base case: we have made more than 6 moves
  if (depth > 6){
    return false;
  }

  //go through squares and try to make a move
  char move[4];
  char directions[] = {'N', 'S', 'E', 'W'};
  for (int row = 0; row < height; row++){
    for (int col = 0; col < width; col++){
      // go through directions to generate a move, to then try out
      for (int i = 0; directions[i] != '\0'; i++){
        generate_move(move, row, col, directions[i]);
        if (make_move(begin, move, height, width)){
          // add move to our solution
          *solution = move[0];
          *++solution = move[1];
          *++solution = move[2];
          *++solution = ',';
          if (find_solution_recursive_helper(begin, end, height, width, solution + 1, depth + 1)){
            return true;
          }
          else {
            // we went through all our moves and could not find a solution
            unmake_move(begin, move);
            // remove from our solution string
            *solution = '\0';
            *--solution = '\0';
            *--solution = '\0';
            *--solution = '\0';
          }
        }
      }
    }
  }
  // no moves were possible at all
  return false;
}


bool find_solution(char** begin, char** end, int height, int width, char* solution){
  // ensure solution is empty
  memset(solution, 0, strlen(solution));
  int depth = 0;
  if (find_solution_recursive_helper(begin, end, height, width, solution, depth)){
    return true;
  }
  return false;
}