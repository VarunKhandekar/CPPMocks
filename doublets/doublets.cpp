#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */

bool valid_step(const char* current_word, const char* next_word){
   int changes = 0;
   for (int i=0; current_word[i]; i++){
      if (current_word[i] != next_word[i]){
         changes++;
         if (changes > 1){
            return false;
         }
      }
   }
   if (!dictionary_search(next_word)){
      return false;
   }
   return true;
}

/* helper */
void convert_to_lower(const char* word, char* word_copy){
   strcpy(word_copy, word);
   while (*word_copy != '\0'){
      *word_copy = tolower(*word_copy);
      word_copy++;
   }
}

bool display_chain(const char** chain, std::ostream& output_stream){
   int i = 0;
   while (*(chain+i) != NULL){
      if (i == 0 || *(chain + i + 1) == NULL){
         output_stream << *(chain+i) << endl;
      }
      else {
         char word_copy[512] = {};
         convert_to_lower(*(chain+i), word_copy);
         output_stream << word_copy << endl;
      }
      if (!output_stream.good()){
         return false;
      }
      i++;
   }

   return true;
}


bool valid_chain(const char** chain){
   int i = 0;
   while (*(chain + i + 1) != NULL){
      // check if it's a valid step
      if (!valid_step(*(chain+i), *(chain+i+1))){
         return false;
      }
      // check if the word appears twice
      int j = i + 1;
      while (*(chain + j) != NULL){
         if(strcmp(*(chain + i), *(chain + j)) == 0){
            return false;
         }
         j++;
      }
      i++;
   }
   return true;
}



bool find_chain_recursive_helper(const char* start_word, const char* end_word, const char** answer_chain, int max_steps, int step){
   // initialise answer string if required
   if (step == 0){
      char* start = new char[strlen(start_word) + 1];
      strcpy(start, start_word);
      answer_chain[0] = start;
      answer_chain[1] = NULL;
   }
   
   // first check we have not exceeded the max number of steps
   if (step > max_steps){
      return false;
   }
   
   // base case:: we have a valid doublet and are at the end of our string
   if (valid_chain(answer_chain) && strcmp(answer_chain[step], end_word) == 0){
      return true;
   }
  
   char* temp_word = new char[strlen(start_word) + 1];
   
   for (int i=0; start_word[i]; i++){
      // try changing a letter
      strcpy(temp_word, answer_chain[step]);
      for (char l='A'; l<='Z'; l++){
         temp_word[i] = l;
         if (valid_step(answer_chain[step], temp_word)){
            // alter our answer chain
            answer_chain[step + 1] = temp_word;
            // NULL terminate
            answer_chain[step + 2] = NULL;
            // try and see if we have the chain
            if (find_chain_recursive_helper(start_word, end_word, answer_chain, max_steps, step + 1)){
               return true;
            }
         }
      }
   }
   return false;
}



bool find_chain(const char* start_word, const char* end_word, const char** answer_chain, int max_steps){
   int step = 0;
   if (find_chain_recursive_helper(start_word, end_word, answer_chain, max_steps, step)){
      return true;
   }
   return false;
}