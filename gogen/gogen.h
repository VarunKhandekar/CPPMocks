#ifndef GOGEN_H
#define GOGEN_H

/*  Pre-supplied functions for loading and printing board and word list */

char **load_board(const char *filename);
bool save_board(char **board, const char *filename);
void print_board(char **board);

char **load_words(const char *filename);
void print_words(char **words);

void delete_board(char **board);
void delete_words(char **words);

/* -----------8<-------------8<------------8<--------------8<------------ */

/* add your function prototypes here */


bool get_position(char**, char, int&, int&);

bool valid_solution(char**, char**);

void update(char**, char, Mask&);

void neighbourhood_intersect(Mask&, Mask&);

bool solve_board(char**, char**);

#endif