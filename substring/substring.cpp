#include <iostream>
#include "substring.h"

int str_len(const char* word){
    int count = 0;
    while (*word != '\0'){
        count++;
        word++;
    }
    return count;
}

bool is_prefix(const char* word1, const char* word2){
    // check to ensure word 1 is longer than word 2
    if (str_len(word1) > str_len(word2)){
        return false;
    }
    // check if we have reached the end of word1 with sentinel character. If so, word1 must be a prefix
    if (word1[0] == '\0'){
        return true;
    }
    if (word1[0] == word2[0]){
        if (is_prefix(word1 + 1, word2 + 1)){ //pointer arithmetic
            return true;
        }
    }
    
    return false;
}


int substring_position(const char* word1, const char* word2){
    if (word1[0] == '\0'){
        return 0;
    }
    int pos = 0;
    while (*word2 != '\0'){
        if (is_prefix(word1, word2)){
            return pos;
        }
        else {
            ++pos;
            ++word2;
        }
    }
    return -1;
}

int substring_position_recursion(const char* word1, const char* word2){
    // base case: we found the substring
    if (is_prefix(word1, word2)){
        return 0;
    }
    // base case: we didn't find the substring
    else if (*word2 == '\0'){
        return -1;
    }
    // handle if we didn't find it
    else if (substring_position_recursion(word1, word2 + 1) < 0){
        return -1;
    }
    // handle if we are still looking for it
    else {
        return substring_position_recursion(word1, word2 + 1) + 1;
    }   
}