#ifndef WORDS_H
#define WORDS_H

/* A function to reverse the order of a string */
void reverse(const char*, char*);

/* A function to check if two strings are identical, ignoring punctuation and white space (case insensitive) */
bool compare(const char*, const char*);

/* A function to check if a string is a palindrome */
bool palindrome(const char*);

/* A function to clean up a string of punctuation and whitespace, and convert all letters to upper case */
void clean_string(const char*, char*);

/* A function to sort a string's letters in alphabetical order */
void string_sort(char*);

/* A function to check if two strings are anagrams of one another */
bool anagram(const char*, const char*);

#endif