#ifndef BRAILLE_H
#define BRAILLE_H

/* A function to convert a character into a braille string. 
 * Returns an integer representing the number of non null characters in the braille encoding */
int encode_character(char, char*);

/* A function to encode a full sentence into braille */
void encode(const char*, char*);

/* A function to print the given braille cells in a provided braille string
 * The cells are supplied through an integer array of size 2 */
void print_braille_row(const char*, int*, std::ostream&);

/* A function to print out a braille string in a 3x2 format */
void print_braille(const char*, std::ostream&);

#endif