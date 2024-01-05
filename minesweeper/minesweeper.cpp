#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */

bool is_complete(char mines[9][9], char revealed[9][9]){
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      // icheck non mine squares and ensure they have been revealed
      if (mines[row][col] != '*'){
        if (revealed[row][col] == '?'){
          return false;
        }
      }
    }
  }

  return true;
}

int pos_to_row(const char* pos){
  if (isupper(pos[0]) != 0){
    return pos[0]-65;
  }
  return -1;
}

int pos_to_col(const char* pos){
  if (isdigit(pos[1]) != 0){
    return pos[1]-49;
  }
  return -1;
}

int count_mines(const char* position, char mines[9][9]){
  int pos_row = pos_to_row(position);
  int pos_col = pos_to_col(position);
  int count = 0;
  for (int row_delta = -1; row_delta <= 1; row_delta++){
    for (int col_delta = -1; col_delta <= 1; col_delta++){
      // ignore the case of us testing position itself
      if (row_delta == 0 && col_delta == 0){
        continue;
      }
      int test_row = pos_row + row_delta;
      int test_col = pos_col + col_delta;
      // we are testing a valid board square
      if (!(test_row < 0) && !(test_col < 0) && !(test_row > 8) && !(test_col > 8)){
        if (mines[test_row][test_col] == '*'){
          count++;
        }
      }
    }
  }

  return count;
}

MoveResult make_move(const char* position, char mines[9][9], char revealed[9][9]){
  int pos_row = pos_to_row(position);
  int pos_col = pos_to_col(position);

  // position is invalid
  if (pos_row < 0 || pos_col < 0 || pos_row > 8 || pos_col > 8){
    return INVALID_MOVE;
  }

  // move is redundant
  if (revealed[pos_row][pos_col] != '?'){
    return REDUNDANT_MOVE;
  }

  // check third character of submitted move if it's there
  if (strlen(position) == 3){
    // ensure it is a * first
    if (position[2] != '*'){
      return INVALID_MOVE;
    }
    // assign flag
    else {
      revealed[pos_row][pos_col] = '*';
      return VALID_MOVE;
    }
  }
  
  // now consider the non-flagging cases. This is where we might complete the board
  // we have blown up a mine
  if (mines[pos_row][pos_col] == '*'){
    return BLOWN_UP;
  }
  // we have uncovered a square that is adjacent to mines
  if (count_mines(position, mines) > 0){
    revealed[pos_row][pos_col] = (count_mines(position, mines) + 48); //converting int to appropriate char
    // check for completion of the board
    if (is_complete(mines, revealed)){
      return SOLVED_BOARD;
    }
    return VALID_MOVE;
  }
  // we have uncovered a square that is not adjacent to mines
  if (count_mines(position, mines) == 0){
    revealed[pos_row][pos_col] = ' ';
    // check for completion of the board
    if (is_complete(mines, revealed)){
      return SOLVED_BOARD;
    }
    // recursively clear adjacent, non-mine squares    
    for (int row_delta = -1; row_delta <= 1; row_delta++){
      for (int col_delta = -1; col_delta <= 1; col_delta++){
        // ignore the case of us testing position itself
        if (row_delta == 0 && col_delta == 0){
          continue;
        }
        int new_row = pos_row + row_delta;
        int new_col = pos_col + col_delta;
        char row_char = new_row + 65;
        char col_char = new_col + 49;
        char new_position[3] = {row_char, col_char};
        MoveResult status = make_move(new_position, mines, revealed);
        if (status == SOLVED_BOARD){
          return status;
        }
      }
    }
    return VALID_MOVE;
  }

  return VALID_MOVE;
}


int count_unrevealed(const char* position, char revealed[9][9]){
  int pos_row = pos_to_row(position);
  int pos_col = pos_to_col(position);
  int count = 0;
  for (int row_delta = -1; row_delta <= 1; row_delta++){
    for (int col_delta = -1; col_delta <= 1; col_delta++){
      // ignore the case of us testing position itself
      if (row_delta == 0 && col_delta == 0){
        continue;
      }
      int test_row = pos_row + row_delta;
      int test_col = pos_col + col_delta;
      // we are testing a valid board square
      if (!(test_row < 0) && !(test_col < 0) && !(test_row > 8) && !(test_col > 8)){
        if (revealed[test_row][test_col] == '?'){
          count++;
        }
      }
    }
  }

  return count;
}

bool find_safe_move(char revealed[9][9], char* move){
  for (int row = 0; row < 9; row++){
    for (int col = 0; col < 9; col++){
      //explore unrevealed squares
      if (revealed[row][col] == '?'){
        // check neighbours.
        for (int row_delta = -1; row_delta <= 1; row_delta++){
          for (int col_delta = -1; col_delta <= 1; col_delta++){
            // ignore the case of us testing position itself
            if (row_delta == 0 && col_delta == 0){
              continue;
            }
            int test_row = row + row_delta;
            int test_col = col + col_delta;
            // we are testing a valid board square
            if (!(test_row < 0) && !(test_col < 0) && !(test_row > 8) && !(test_col > 8)){
              // If one is a number, check how many ? and how many * are next to it.
              if (isdigit(revealed[test_row][test_col]) != 0){
                char row_char = test_row + 65;
                char col_char = test_col + 49;
                char position[3] = {row_char, col_char};
                int unrevealed_count = count_unrevealed(position, revealed);
                int mine_count = count_mines(position, revealed);
                int square_number = revealed[test_row][test_col] - 48;

                char original_row_char = row + 65;
                char original_col_char = col + 49;
                // If * == number, we can uncover. 
                if (mine_count == square_number){
                  *move = original_row_char;
                  *++move = original_col_char;
                  *++move = '\0';
                  //cout << move << endl;
                  return true;
                }
                // If ? is 1 and square count exceeds star by exactly 1, we can * it, otherwise move on
                else if (square_number == (mine_count + 1) && unrevealed_count == 1){
                  *move = original_row_char;
                  *++move = original_col_char;
                  *++move = '*';
                  *++move = '\0';
                  //cout << move << endl;
                  return true;
                }
              }
            }
          }
        }
      }
    }
  }
  *move = '\0';
  return false;
}
