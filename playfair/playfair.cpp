#include <iostream>
#include <cctype>
#include "playfair.h"

void prepare(const char* input, char* output){
    int count = 0;
    while (*input != '\0'){
        char ch = *input;
        // ignore blanks and punctuation
        if (std::isalnum(ch) != 0){
            // check for letters
            if (std::isalpha(ch) != 0){
                *output = std::toupper(ch);
            }
            // check for digits
            else {
                *output = ch;
            }
            // increment count and move pointer along
            count++;
            output++;

        }

        input++;
    }
    // add 'X' at the end if we have an odd number of letters in our output
    if (count%2 != 0){
        *output = 'X';
        output++;
    }
    // add sentinel
    *output = '\0';
}


bool occurs_before(const char* str, char letter, int pos){
    for (int i=0; i<pos; i++){
        if (str[i] == letter){
            return true;
        }
    }
    return false;
}


void grid(const char* codeword, char square[][6]){
    // initialise empty string array to keep track of what is in our grid
    char working_word[37] = {};

    char alphanumeric[37] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    // start by adding our codeword
    int working_word_pos = 0;
    for (int row = 0; row < 6; row++){
        for (int col = 0; col < 6; col++){
            while (*codeword != '\0'){
                if (!occurs_before(working_word, *codeword, working_word_pos)){
                    // the letter doesn't exist in our grid yet so add it
                    square[row][col] = *codeword;
                    working_word[working_word_pos] = *codeword;
                    // move codeword pointer along and increment our working word position
                    codeword++;
                    working_word_pos++;
                    //exit the while loop to move onto the next square in our grid
                    break;
                }
                else {
                    codeword++;
                }
            }
        }
    }
    std::cout<< working_word << std::endl;
    std::cout<< alphanumeric << std::endl;
    // now add remaining alphanumeric values
    for (int row = 0; row < 6; row++){
        for (int col = 0; col < 6; col++){
            //ensure we are looking at non-occupied squares
            if (std::isalnum(square[row][col]) == 0){
                for (int i = 0; i < 36; i++){
                    if (!occurs_before(working_word, alphanumeric[i], working_word_pos)){
                        // the alphanumeric value doesn't exist in our grid yet so add it
                        square[row][col] = alphanumeric[i];
                        working_word[working_word_pos] = alphanumeric[i];
                        // increment our working word position
                        working_word_pos++;
                        //exit the while loop to move onto the next square in our grid
                        break;
                    }
                }
            }  
        }
    }
}

void bigram(char square[][6], const char inchar1, const char inchar2, char& outchar1, char& outchar2){
    int row_ch1;
    int col_ch1;
    int row_ch2;
    int col_ch2;
    
    for (int row = 0; row < 6; row++){
        for (int col = 0; col < 6; col++){
            if (square[row][col] == inchar1){
                row_ch1 = row;
                col_ch1 = col;
            }
            if (square[row][col] == inchar2){
                row_ch2 = row;
                col_ch2 = col;
            }
        }
    }

    outchar1 = square[row_ch1][col_ch2];
    outchar2 = square[row_ch2][col_ch1];
}


void encode(char square[][6], const char* prepared, char* encoded){
    // base case:: we have reached the end of the string
    if (*prepared == '\0'){
        return;
    }
    
    // work in batches of 2
    char temp_letters[3] = {};
    for (int i=0; i<2; i++){
        temp_letters[i] = *prepared;
        prepared++;
    }

    // we now have the bigram. Now encode it
    char outchar1, outchar2;
    bigram(square, temp_letters[0], temp_letters[1], outchar1, outchar2);

    // add the encodings to encoded
    *encoded = outchar1;
    *++encoded = outchar2;

    // move on; prepared is already pointing at the correct memory location. encoded needs incrementing
    encode(square, prepared, encoded+1);
}


void decode(char square[][6], const char* encoded, char* decoded){
    // base case:: we have reached the end of the string
    if (*encoded == '\0'){
        return;
    }
    
    // work in batches of 2
    char temp_letters[3] = {};
    for (int i=0; i<2; i++){
        temp_letters[i] = *encoded;
        encoded++;
    }

    // we now have the bigram. Now encode it
    char outchar1, outchar2;
    bigram(square, temp_letters[0], temp_letters[1], outchar1, outchar2);

    // add the encodings to encoded
    *decoded = outchar1;
    *++decoded = outchar2;

    // move on; encoded is already pointing at the correct memory location. decoded needs incrementing
    decode(square, encoded, decoded+1);
}
