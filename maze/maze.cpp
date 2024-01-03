#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows]; //pointer that points to an array of pointers on the heap
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns]; //each pointer on the heap now points to a char array on heap
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

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line); //copy each line from the file into the char array on heap that reperesents a particular row
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
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
      cout << m[r][c];
    cout << endl;
  }
}


bool find_marker(char ch, char** maze, int height, int width, int& row, int& column){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (maze[r][c] == ch){
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



void map_direction(const char c, int& row_delta, int& column_delta){
  switch(c){
    case 'N': row_delta = -1;
              column_delta = 0;
              break;
    case 'S': row_delta = 1;
              column_delta = 0;
              break;
    case 'E': row_delta = 0;
              column_delta = 1;
              break;
    case 'W': row_delta = 0;
              column_delta = -1;
              break;
    default: row_delta = 0;
             column_delta = 0;
             break;
  }
}

bool is_valid_move(char** maze, int height, int width, int row, int column){
  // off the board
  if (row < 0 || column < 0 || row >= height || column >= width){
    return false;
  }
  // blocked by a hedge or another character
  if (maze[row][column] == '-' || maze[row][column] == '+' || maze[row][column] == '|' || maze[row][column] == '#'){
    return false;
  }
  return true;
}

bool valid_solution(const char* path, char** maze, int height, int width){
  // get start location
  int row, column;
  if (!find_marker('>', maze, height, width, row, column)){
    cerr << "There is no entrance to the maze!" << endl;
    return false;
  }
  // get end location
  int end_row, end_column;
  if (!find_marker('X', maze, height, width, end_row, end_column)){
    cerr << "There is no exit to the maze!" << endl;
    return false;
  }

  // set up row and column incrementers
  int row_delta, column_delta;
  while (*path != '\0'){
    map_direction(*path, row_delta, column_delta);
    row += row_delta;
    column += column_delta;
    // check if we have hit a hedge
    if (!is_valid_move(maze, height, width, row, column)){
      return false;
    }
    // check if we have reached the exit
    if (row == end_row && column == end_column){
      return true;
    }
    path++;
  }

  // we failed to reach the exit, there the path is not a valid solution
  return false;
}


char opposite_direction(char c){
  switch(c){
    case 'N': return 'S';
    case 'S': return 'N';
    case 'E': return 'W';
    case 'W': return 'E';
    default: return '\0';
  }
}


bool find_path_recursive_helper(char** maze, int height, int width, int row, int column, char* path, char end){
  // set up our directions
  char directions[5] = "NSEW";
  
  // base case: we have found a valid solution
  if (maze[row][column] == end){
    // final hashtagging
    maze[row][column] = '#';
    return true;
  }

  // try making a move
  for (int i=0; i<4; i++){
    // try making a move
    int row_delta, column_delta;
    map_direction(directions[i], row_delta, column_delta);

    // the path is clear, so try a move forward
    if (is_valid_move(maze, height, width, row + row_delta, column + column_delta)){
      // add the letter to our path and move on
      *path = directions[i];
      // add # 
      maze[row][column] = '#';
      
      if (find_path_recursive_helper(maze, height, width, row+row_delta, column+column_delta, path+1, end)){
        // by here, we have successfully continued our recursion to completion.
        return true;
      }
      else {
        // by here, we failed to make a move i.e. the path was blocked       
        // remove the last entry to our path
        *path = '\0';
        maze[row][column] = ' ';
      }
    }
  }
  return false;
}

string find_path(char** maze, int height, int width, char start, char end){
  // initialise an array and an appropriate pointer for our path
  char path[(height*width)+1] = {};
  
  // get our start point in the maze
  int row, column;
  find_marker(start, maze, height, width, row, column);

  // we found a path
  if (find_path_recursive_helper(maze, height, width, row, column, path, end)){
    return static_cast<string>(path);
  }
  
  // we failed to find a path
  else {
    return "No solution";
  }
}

