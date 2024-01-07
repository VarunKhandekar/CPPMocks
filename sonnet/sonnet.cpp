#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */
int count_words(const char* line){
  char* token;
  char line_copy[512] = {};
  strcpy(line_copy, line);
  int count = 0;
  token = strtok(line_copy, "- ");
  while (token != nullptr){
    token = strtok(nullptr, "- ");
    count++;
  }
  return count;
}


bool contains_vowel(char* word){
  while (*word != '\0'){
    if(toupper(*word) == 'A' || toupper(*word) == 'E' || toupper(*word) == 'I' || toupper(*word) == 'O' || toupper(*word) == 'U'){
      return true;
    }
    word++;
  }
  return false;
}


bool find_phonetic_ending(const char* word, char* phonetic_ending){
  memset(phonetic_ending, 0, strlen(phonetic_ending));
  ifstream in("dictionary.txt");
  char line[512];
  char* token;

  if (in.fail()){
    cerr << "Can't open dictionary!" << endl;
    return false;
  }
  
  while (in){
    in.getline(line, 512);
    token = strtok(line, " ");
    if (strcmp(word, token) == 0){
      while (token != nullptr){
        if (contains_vowel(token)){
          memset(phonetic_ending, 0, strlen(phonetic_ending));
        }
        strcat(phonetic_ending, token);
        token = strtok(nullptr, " ");
      }
      in.close();
      return true;
    }
  }

  in.close();
  return false;
}


bool find_rhyme_scheme(const char* filename, char* scheme){
  memset(scheme, 0, strlen(scheme));
  ifstream in(filename);
  rhyming_letter(RESET);
  char line[512];
  char ending[512];
  char phonetic_ending[512];
  
  int word_count;

  if (in.fail()){
    cerr << "Can't open file!" << endl;
    return false;
  }

  while (in){
    in.getline(line, 512);
    word_count = count_words(line);
    if(get_word(line, word_count, ending)){
      if (find_phonetic_ending(ending, phonetic_ending)){
        char letter;
        letter = rhyming_letter(phonetic_ending);
        *scheme = letter;
        scheme++;
      }
      else {
        return false;
      }
    }
    else{
      return false;
    }
  }
  *scheme = '\0';
  in.close();
  return true;
}


string identify_sonnet(const char* filename){
  char scheme[512] = {};
  find_rhyme_scheme(filename, scheme);

  if (strcmp(scheme, "ababcdcdefefgg") == 0){
    return "Shakespearean";
  }
  else if (strcmp(scheme, "abbaabbacdcdcd") == 0){
    return "Petrarchan";
  }
  else if (strcmp(scheme, "ababbcbccdcdee") == 0){
    return "Spenserian";
  }
  else {
    return "Unknown";
  }
}
