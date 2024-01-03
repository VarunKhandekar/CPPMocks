#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "braille.h"

int encode_character(char ch, char* braille){
    // ensure braille is empty before starting. Don't need to do strlen + 1 as we want everything to be null anyway.
    std::memset(braille, 0, std::strlen(braille));

    // letters
    if (std::isalpha(ch) != 0){
        // check if uppercase
        if (std::isupper(ch) != 0){
            //add special upper case symbol
            strcat(braille, ".....0");
        }
        switch(std::tolower(ch)){
            case 'a': strcat(braille, "0.....");
                      break;
            case 'b': strcat(braille, "00....");
                      break;
            case 'c': strcat(braille, "0..0..");
                      break;
            case 'd': strcat(braille, "0..00.");
                      break;
            case 'e': strcat(braille, "0...0.");
                      break;
            case 'f': strcat(braille, "00.0..");
                      break;
            case 'g': strcat(braille, "00.00.");
                      break;
            case 'h': strcat(braille, "00..0.");
                      break;
            case 'i': strcat(braille, ".0.0..");
                      break;
            case 'j': strcat(braille, ".0.00.");
                      break;
            case 'k': strcat(braille, "0.0...");
                      break;
            case 'l': strcat(braille, "000...");
                      break;
            case 'm': strcat(braille, "0.00..");
                      break;
            case 'n': strcat(braille, "0.000.");
                      break;
            case 'o': strcat(braille, "0.0.0.");
                      break;
            case 'p': strcat(braille, "0000..");
                      break;
            case 'q': strcat(braille, "00000.");
                      break;
            case 'r': strcat(braille, "000.0.");
                      break;
            case 's': strcat(braille, ".000..");
                      break;
            case 't': strcat(braille, ".0000.");
                      break;
            case 'u': strcat(braille, "0.0..0");
                      break;
            case 'v': strcat(braille, "000..0");
                      break;
            case 'w': strcat(braille, ".0.000");
                      break;
            case 'x': strcat(braille, "0.00.0");
                      break;
            case 'y': strcat(braille, "0.0000");
                      break;
            case 'z': strcat(braille, "0.0.00");
                      break;
            default: break;
        }
    }
    // numbers
    else if (std::isdigit(ch) != 0){
        // add special number coding
        strcat(braille, "..0000");
        switch(ch){
            case '1': strcat(braille, "0.....");
                      break;
            case '2': strcat(braille, "00....");
                      break;
            case '3': strcat(braille, "0..0..");
                      break;
            case '4': strcat(braille, "0..00.");
                      break;
            case '5': strcat(braille, "0...0.");
                      break;
            case '6': strcat(braille, "00.0..");
                      break;
            case '7': strcat(braille, "00.00.");
                      break;
            case '8': strcat(braille, "00..0.");
                      break;
            case '9': strcat(braille, ".0.0..");
                      break;
            case '0': strcat(braille, ".0.00.");
                      break;
            default: break;
        }
    }
    // punctuation
    else if (std::ispunct(ch) != 0){
        switch(ch){
            case '.': strcat(braille, ".0..00");
                      break;
            case ',': strcat(braille, ".0....");
                      break;
            case ';': strcat(braille, ".00...");
                      break;
            case '-': strcat(braille, "..0..0");
                      break;
            case '!': strcat(braille, ".00.0.");
                      break;
            case '?': strcat(braille, ".00..0");
                      break;
            case '(': strcat(braille, ".00.00");
                      break;
            case ')': strcat(braille, ".00.00");
                      break;
            default: break;
        }
    }
    else {
        strcat(braille, "00....");
    }

    return std::strlen(braille);
}


void encode(const char* plaintext, char* braille){
    // base case: we are at the end of plaintext
    if (*plaintext == '\0'){
        return;
    }
    // create a holder to store the given character's braille
    char temp_braille[512] = {};
    encode_character(*plaintext, temp_braille);
    
    // add to our 'working output'
    std::strcat(braille, temp_braille);
    encode(plaintext+1, braille);
}


void print_braille_row(const char* braille, int elements[2], std::ostream& os){
    for (int i=0; braille[i] != '\0'; i++){
        if (i%6 == elements[0] - 1){
            os << braille[i];
        }
        if (i%6 == elements[1] - 1){
            os << braille[i];
            os << " ";
        }
    }
    os << std::endl;
}


void print_braille(const char* plaintext, std::ostream& os){
    // get our braille output
    char braille[512] = {};
    encode(plaintext, braille);

    // go through our braille
    int elements[2] = {1, 4};
    print_braille_row(braille, elements, os);

    elements[0] = 2;
    elements[1] = 5;
    print_braille_row(braille, elements, os);

    elements[0] = 3;
    elements[1] = 6;
    print_braille_row(braille, elements, os);

    // now output the plaintext itself
    while (*plaintext != '\0'){
        // encoded char can be of length 6 or 12
        if (encode_character(*plaintext, braille) == 6){
            os << *plaintext << "  ";
        }
        else if (encode_character(*plaintext, braille) == 12){
            os << "   " << *plaintext << "  ";
        }
        plaintext++;
    }
    os << std::endl;
}
