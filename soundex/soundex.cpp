#include <iostream>
#include <cctype>
#include "soundex.h"


char mapLetter(char c){
    // add some error checking
    char mapped_char;
    switch(c){
        case 'b': return '1';
        case 'f': mapped_char = '1';
                  break;
        case 'p': mapped_char = '1';
                  break;
        case 'v': mapped_char = '1';
                  break;
        case 'c': mapped_char = '2';
                  break;
        case 'g': mapped_char = '2';
                  break;
        case 'j': mapped_char = '2';
                  break;
        case 'k': mapped_char = '2';
                  break;
        case 'q': mapped_char = '2';
                  break;
        case 's': mapped_char = '2';
                  break;
        case 'x': mapped_char = '2';
                  break;
        case 'z': mapped_char = '2';
                  break;
        case 'd': mapped_char = '3';
                  break;
        case 't': mapped_char = '3';
                  break;
        case 'l': mapped_char = '4';
                  break;
        case 'm': mapped_char = '5';
                  break;
        case 'n': mapped_char = '5';
                  break;
        case 'r': mapped_char = '6';
                  break;
        default: mapped_char = '\0';
                  break;           
    }
    return mapped_char;
}


bool valid_surname(const char* surname){
    // check surname is not empty
    if (*surname == '\0'){
        return false;
    }

    while (*surname != '\0'){
        if (std::isalpha(*surname) == 0){
            return false;
        }
        surname++;
    }
    return true;
}


void encode(const char* surname, char soundex[]){
    // set first letter of soundex to equal first letter of surname (in upper case), and last letter to equal the required sentinel character
    soundex[0] = std::toupper(*surname);
    soundex[4] = '\0';
    surname++; //move pointer to the second element of the surname

    for (int i = 1; i < 4; i++){
        while (*surname != '\0'){
            char coding = mapLetter(*surname);
            surname++; //move to next letter in surname
            if (coding == '\0'){ //if no coding, skip
                continue;
            }
            
            //check previous char is not the same in soundex
            if (soundex[i-1] != coding){
                soundex[i] = coding; // add if it isn't
                i++; //move to next char in soundex
            }

            //exit if we have populated the soundex
            if (i == 4){
                return;
            }
        }
        //blank fill with zeros if needed
        soundex[i] = '0';
    }
    return;
}


bool compare(const char* one, const char* two){
    // base case: we have reached end of both strings
    if (*one == '\0' && *two == '\0'){
        return true;
    }
    // check if the same letter
    if (*one == *two){
        return compare(one+1, two+1);
    }
    // if not same letter, return false
    else {
        return false;
    }
}


void reset_string(char* string){
    while (*string != '\0'){
        *string = '\0';
        string++;
    }
}


int count(const char* surname, const char* sentence){
    char surname_soundex[5]= {};
    encode(surname, surname_soundex);

    char sentence_surname_soundex[5] = {};
    int soundex_count = 0;
    char sentence_surname[196] = {};

    int i = 0; //initialise an iterator to add to sentence_surname

    // go through sentence. Add letters to a surname string
    while (*sentence != '\0'){
        // check if we have hit something that isn't a letter
        if (std::isalpha(*sentence) == 0){
            if (valid_surname(sentence_surname)){
                encode(sentence_surname, sentence_surname_soundex);
                //std::cout << sentence_surname << ": " << sentence_surname_soundex << " vs " << surname_soundex << std::endl;
                if (compare(surname_soundex, sentence_surname_soundex)){
                        soundex_count++;
                }
            }
            // reset our iterator and sentence_surname string
            i = 0;
            reset_string(sentence_surname);
        }

        else {
            sentence_surname[i] = *sentence;
            i++;
        }
        sentence++;
    }

    return soundex_count;
}