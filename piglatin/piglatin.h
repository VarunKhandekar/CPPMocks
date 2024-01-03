#ifndef PIGLATIN_H
#define PIGLATIN_H

int findFirstVowel(const char*);

void translateWord(const char*, char*);

void translateStream(std::istream&, std::ostream&);

#endif