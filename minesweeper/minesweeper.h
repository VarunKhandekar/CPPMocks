#ifndef MINESWEEPER_H
#define MINESWEEPER_H

enum MoveResult { INVALID_MOVE=-3, REDUNDANT_MOVE=-2, BLOWN_UP=-1, SOLVED_BOARD=1, VALID_MOVE=0 };

void load_board(const char *filename, char board[9][9]);
void display_board(const char board[9][9]);
void initialise_board(char board[9][9]);


bool is_complete(char[9][9], char[9][9]);

int count_mines(const char*, char[9][9]);

MoveResult make_move(const char*, char[9][9], char[9][9]);

bool find_safe_move(char[9][9], char*);

#endif