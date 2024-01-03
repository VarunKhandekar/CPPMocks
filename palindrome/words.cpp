#include <iostream>
#include <cstring>
#include <cctype>
#include "words.h"

void reverse(const char* str1, char* str2){
    int length = std::strlen(str1);
    for (int i = (length - 1); i >= 0; i--){
        *str2 = *(str1 + i);
        str2++;
    }
    // add sentinel on the end. We are already pointing at thre end of the string now
    *str2 = '\0';
}


bool compare(const char* str1, const char* str2){
    // check if we have punctuation. If so, move on to the next character
    while(std::isalpha(*str1) == 0){
        // check if we have reached the end of the string. If so, no need to push the pointer further along
        if (*str1 == '\0'){
            break;
        }
        str1++;
    }
    while(std::isalpha(*str2) == 0){
        // check if we have reached the end of the string. If so, no need to push the pointer further along
        if (*str2 == '\0'){
            break;
        }
        str2++;
    }
    // base case: we are at end of strings
    if (*str1 == '\0' && *str2 == '\0'){
        return true;
    }
    // now check if the letter is the same
    if (std::toupper(*str1) == std::toupper(*str2)){
        return compare(str1+1, str2+1);
    }
    else {
        return false;
    }
}


bool palindrome(const char* sentence){
    // adding one extra spot for the sentinel character
    char reversed_sentence[std::strlen(sentence) + 1] = {};
    reverse(sentence, reversed_sentence);
    return compare(sentence, reversed_sentence);
}


void clean_string(const char* str, char* str_cleaned){
    while (*str != '\0'){
        if (std::isalpha(*str) != 0){
            *str_cleaned = std::toupper(*str);
            str_cleaned++;
        }
        str++;
    }
    *str_cleaned = '\0';
}


void string_sort(char* str){
    // base case
    if (*str == '\0'){
        return;
    }

    string_sort(str+1);
    while (*(str+1) != '\0'){
        if (*str > *(str+1)){
            // make swap
            char temp;
            temp = *(str+1);
            *(str+1) = *str;
            *str = temp;
        }
        str++;
    }
}


bool anagram(const char* str1, const char* str2){
    char sorted_str1[std::strlen(str1) + 1] = {};
    char sorted_str2[std::strlen(str2) + 1] = {};
    clean_string(str1, sorted_str1);
    clean_string(str2, sorted_str2);
    string_sort(sorted_str1);
    string_sort(sorted_str2);

    return compare(sorted_str1, sorted_str2);
}