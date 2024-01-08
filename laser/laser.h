#ifndef LASER_H
#define LASER_H
/* pre-supplied function and type declarations */

char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **array, int rows);
char **load_board(const char *filename, int &height, int &width);
void print_board(char **board, int height, int width);

enum Direction {NORTH, EAST, SOUTH, WEST};

/* add your own function and type declarations here */

bool find_laser(char**, int, int, int&);

char mirror_label(char**, int, int, int, int);

/* helper */
void direction_to_delta(Direction , int& , int& );

bool shoot(char**, int, int, char*, int&, int&);

bool solve(char**, int, int, const char*);

#endif