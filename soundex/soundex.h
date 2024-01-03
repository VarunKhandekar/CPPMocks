#ifndef SOUNDEX_H
#define SOUNDEX_H

/* A function to map a char to the appropriate soundex encoding */
char mapLetter(char);

/* A function to check if a surname just contains letters */
bool valid_surname(const char*);

/* A function to convert a surname into a soundex encoding */
void encode(const char*, char[]);

/* A function to check if two strings are the same */
bool compare(const char*, const char*);

/* A function to reset a string to null */
void reset_string(char*);

/* A function to count the number of occurrences of a surname's soundex encoding in a sentence of surnames */
int count(const char*, const char*);

#endif