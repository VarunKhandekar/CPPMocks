#include <iostream>
#include <cctype>
#include <fstream>
#include <cstring>
#include "piglatin.h"

int findFirstVowel(const char* word){
    for (int i=0; i < static_cast<int>(std::strlen(word)); i++){
        char ch = std::tolower(word[i]);
        if (i == 0 || i == static_cast<int>(strlen(word)-1)){
            if (ch == 'a'||ch == 'e'||ch == 'i'||ch == 'o'||ch == 'u'){
                return i;
            }
        }
        else {
            if (ch == 'a'||ch == 'e'||ch == 'i'||ch == 'o'||ch == 'u'||ch == 'y'){
                return i;
            }   
        }
    }
    
    return -1;
}

void translateWord(const char* english, char* piglatin){
    // clean up piglatin before using it
    std::memset(piglatin, 0, std::strlen(piglatin));

    // check if it begins with a digit
    if (std::isdigit(*english) != 0){
        std::strcat(piglatin, english);
        return;
    }
    int first_vowel = findFirstVowel(english);
    // check if the first letter is a vowel
    if (first_vowel == 0){
        std::strcat(piglatin, english);
        std::strcat(piglatin, "way");
        return;
    }
    // check for the case when the first letter is not a vowel
    if (first_vowel != 0){
        // add from first vowel onwards
        for (int i = first_vowel; i < static_cast<int>(std::strlen(english)); i++){
            char ch = english[i];
            if (i == first_vowel && std::isupper(*english) != 0){
                *piglatin = std::toupper(ch);
            }
            else {
                *piglatin = std::tolower(ch);
            }
            piglatin++;
        }
        // add the letters up to the first vowel
        for (int i = 0; i < first_vowel; i++){
            char ch = english[i];
            *piglatin = std::tolower(ch);
            piglatin++;
        }
        std::strcat(piglatin, "ay");
        return;
    }
    return;
}


void translateStream(std::istream& inputStream, std::ostream& outputStream){
    char word[64] = {};
    char piglatin[64] = {};
    char ch;

    inputStream.get(ch);
    //base case: end of file
    if (inputStream.eof()){
        return;
    }
    // if we have punctuation or new line
    if (std::isalnum(ch) == 0){
         outputStream << ch; 
    }
     // case where we have letters
    else {
        int i = 0;
        while (std::isalnum(ch) != 0){
            // add these letters to our word holder
            word[i] = ch;
            i++;
            inputStream.get(ch);
            if (std::isalnum(ch) == 0){
                inputStream.putback(ch);
            } 
        }
        word[i] = '\0';
        translateWord(word, piglatin);
        outputStream << piglatin;
    }
    // move on to the next non-alphanumeric char, or the next word
    translateStream(inputStream, outputStream);
}