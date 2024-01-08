#ifndef DOUBLETS_H
#define DOUBLETS_H

/*  This pre-supplied function looks up a given word in the dictionary
    of approved words. It is assumed that the input word is in uppercase.
    The function returns true if the word is in the dictionary.
    Otherwise returns false. */

bool dictionary_search(const char *word);

/* add your function prototypes here */


bool valid_step(const char*, const char*);

bool display_chain(const char**, std::ostream&);

bool valid_chain(const char**);

bool find_chain(const char*, const char*, const char**, int);


#endif