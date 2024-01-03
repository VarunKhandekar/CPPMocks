#ifndef MAZE_H
#define MAZE_H

/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);


bool find_marker(char, char**, int, int, int&, int&);

/* Helper function: Convert a compass direction to the appropriate row and column shift needed 
 * e.g. 'N', means a decrement of the row, and no change to the column */
void map_direction(const char, int&, int&);

/* Helper function: returns true if the given position is valid or not (either blocked or off the maze */
bool is_valid_move(char**, int, int, int, int);

bool valid_solution(const char*, char**, int, int);

/* Helper function: returns the opposite direction to the provided direction */
char opposite_direction(char);

bool find_path_recursive_helper(char**, int, int, int, int, char*, char);

std::string find_path(char**, int, int, char, char);


#endif