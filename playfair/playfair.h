#ifndef PLAYFAIR_H
#define PLAYFAIR_H

/* A function to an output string in a format that is suitable for playfair encoding */
void prepare(const char*, char*);

/* A function to check if a given letter appears in a string before a specified position. Returns true if it does */
bool occurs_before(const char*, char, int);

/* A function to produce the 6x6 playfair grid */
void grid(const char*, char[][6]);

/* A function to encode a given bigram (specified as two characters) into another two characters */
void bigram(char[][6], const char, const char, char&, char&);

/* A function to encode a prepared input string using a given encoding grid */
void encode(char[][6], const char*, char*);

/* A function to decode an encoded string using a given encoding grid */
void decode(char[][6], const char*, char*);

#endif