#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
  char **board = allocate_2D_array(5, 6);
  ifstream input(filename);
  assert(input);
  char buffer[512];
  int lines = 0;
  input.getline(buffer, 512);
  while (input && lines < HEIGHT) {
    filter(buffer);
    if (strlen(buffer) != WIDTH)
      cout << "WARNING bad input = [" << buffer << "]" << endl;
    assert(strlen(buffer) == WIDTH);
    strcpy(board[lines], buffer);
    input.getline(buffer, 512);
    lines++;
  }
  input.close();
  return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
  ofstream out(filename); 
  if (!out)
    return false;
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      out << board[r][c];
    }
    out << endl;
  }
  bool result = out.good();
  out.close();
  return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
  char word[512];
  int count = 0;
  ifstream in(filename);
  while (in >> word)
    count++;
  in.close();
  return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
  int count = count_words(filename);
  ifstream in(filename);
  assert(in);
  int n=0;
  char **buffer = new char *[count+1]; // +1 because we NULL terminate ; array of pointers i.e. words on heap
  char word[512];
  for (; (in >> word) && n<count; n++) {
    buffer[n] = new char[strlen(word) + 1];
    strcpy(buffer[n], word);
  }
  buffer[n] = NULL;
  in.close();
  return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      cout << "[" << board[r][c] << "]";
      if (c < WIDTH-1)
	cout << "--";
    }
    cout <<endl;
    if (r < HEIGHT-1) {
      cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
      cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
    }
  }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
  for (int n=0; words[n]; n++) 
    cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
  deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
  int count = 0;
  for (; words[count]; count++);
  deallocate_2D_array(words, count);
}

/* add your own function definitions here */

bool get_position(char** board, char ch, int& row, int& column){
  for (int r = 0; r < HEIGHT; r++){
    for (int c = 0; c < WIDTH; c++){
      if (board[r][c] == ch){
        row = r;
        column = c;
        return true;
      }
    }
  }
  row = -1;
  column = -1;
  return false;
}


bool word_make(char** board, char* word){
  while (*(word+1) != '\0'){
    // create empty false mask
    Mask one(false);
    int row, col;
    if (!get_position(board, *word, row, col)){
      return false;
    }
    one[row][col] = 1;
    one = one.neighbourhood();
    
    // repeat for next letter
    Mask two(false);
    int next_row, next_col;
    if (!get_position(board, *(word+1), next_row, next_col)){
      return false;
    }
    two[next_row][next_col] = 1;
    two = two.neighbourhood();

    //check intersection
    one *= two;
    if (one.count() == 0){
      return false;
    }
    word++;
  }
  return true;
}


bool valid_solution(char** board, char** words){
  for (int n=0; words[n]; n++){
    if (!word_make(board, words[n])){
      return false;
    }
  }
  return true;
}

void update(char** board, char ch, Mask& mask){
  int r, c;
  // ch is already on our board
  if (get_position(board, ch, r, c)){
    mask.set_all(false);
    mask[r][c] = true;
    return;
  }
  // reduce places the letter can go
  for (int row = 0; row < HEIGHT; row++){
    for (int col = 0; col < WIDTH; col++){
      // check if letter
      if (isalpha(board[row][col]) != 0){
        // check what letter
        if (board[row][col] != ch){
          mask[row][col] = false;
        }
      }
    }
  }
  // only one place the letter can go
  if (mask.count() == 1){
    mask.get_position(true, r, c);
    board[r][c] = ch;
  }
}



void neighbourhood_intersect(Mask& one, Mask& two){
  // get neighbours of one and intersect with two. Set two to this
  Mask nbr_one = one.neighbourhood();
  nbr_one *= two;
  two = nbr_one;

  // repeat but the opposite
  Mask nbr_two = two.neighbourhood();
  nbr_two *= one;
  one = nbr_two;
}


int get_empty_squares(char** board){
  int count = 0;
  for (int r = 0; r < HEIGHT; r++){
    for (int c = 0; c < WIDTH; c++){
      if (board[r][c] == '.'){
        count++;
      }
    }
  }
  return count;
}


bool words_solve(char** board, char** words, Mask* masks, int empty_squares_old){
  // base case: check if we have a solution
  if (valid_solution(board, words)){
    return true;
  }
  
  // go through words to refine
  for (int n=0; words[n]; n++){
    // refine relevant masks by going through each word
    for (int i=0; words[n][i+1] != '\0'; i++){
      char first_ch, next_ch;
      first_ch = words[n][i];
      next_ch = words[n][i+1];

      int first_ch_int = first_ch - 65;
      int next_ch_int = next_ch - 65;

      neighbourhood_intersect(masks[first_ch_int], masks[next_ch_int]);
      update(board, first_ch, masks[first_ch_int]);
      update(board, next_ch, masks[next_ch_int]);
    }
  }
  int empty_squares_new = get_empty_squares(board);

  // we made no further improvements
  if (empty_squares_new == empty_squares_old){
    return false;
  }
  // some improvements were made, so try again
  if (words_solve(board, words, masks, empty_squares_new)){
    return true;
  }
  return false;
}


bool blanks_solve(char** board, char** words){
  // base case: check if we have a solution
  if (valid_solution(board, words)){
    print_board(board);
    return true;
  }
  // go through board to find blanks
  for (int row = 0; row < HEIGHT; row++){
    for (int col = 0; col < WIDTH; col++){
      if (board[row][col] == '.'){
        //try adding a letter not on our board
        for (char ch = 'A'; ch <= 'Y'; ch++){
          int r,c;
          if (!get_position(board, ch, r, c)){
            board[row][col] = ch;
            // try next square
            if (blanks_solve(board, words)){
              return true;
            }  
          }
        }
        // no characters worked, so there is no solution. Revert board
        board[row][col] = '.';
        return false;
      }
    }
  }
  return false;
}


bool solve_board(char** board, char** words){
  // initialise masks
  Mask masks[25];
  char ch = 'A';
  for (int i=0; i < 25; i++){
    update(board, static_cast<char>(ch + i), masks[i]);
  }
  int empty_squares = get_empty_squares(board);

  // see if going refinement via words solves the problem
  if (words_solve(board, words, masks, empty_squares)){
    return true;
  }
  // now try brute forcing the remaining blanks
  if (blanks_solve(board, words)){
    return true;
  }

  return false;
}
