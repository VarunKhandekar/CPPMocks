#include <iostream>
#include <cstring>
#include <cmath>
#include "correct.h"


using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary);


/* converts a binary string representation into a character */
char binary_to_ascii(const char *binary);


void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}


char binary_to_ascii(const char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}


/* now add your own functions here */
void text_to_binary(const char* str, char* binary){
  // base case: we are at the end of the string
  if (*str == '\0'){
    *binary = '\0';
    return;
  }
  char temp_binary[9] = {};
  ascii_to_binary(*str, temp_binary);
  for (int i = 0; i < 8; i++){
    *binary = temp_binary[i];
    binary++;
  }
  // move to next character
  text_to_binary(str+1, binary);
}


void binary_to_text(const char* binary, char* str){
  // base case: we are at the end of the binary string
  if (*binary == '\0'){
    *str = '\0';
    return;
  }

  // extract binary in batches of 8 and convert to ascii
  char temp_binary[9] = {};
  for (int i=0; i<8; i++){
    temp_binary[i] = *binary;
    binary++;
  }
  char ascii_ch = binary_to_ascii(temp_binary);
  // add this char to our output string
  *str = ascii_ch;

  // move on
  binary_to_text(binary, str+1);
}


int ascii_to_int(char ch){
  return ch - 48;
}


char int_to_ascii(int i){
  return i + 48;
}


void add_error_correction(const char* data, char* corrected){
  // base case: we are at the end of the data string
  if (*data == '\0'){
    *corrected = '\0';
    return;
  }

  //extract data bits in batches of 4
  char temp_data[5] = {};
  for (int i=0; i<4; i++){
    temp_data[i] = *data;
    data++;
  }
  //compute check bits
  char c1 = ((ascii_to_int(temp_data[0]) + ascii_to_int(temp_data[1]) + ascii_to_int(temp_data[3]))%2 == 0) ? '0' : '1';
  char c2 = ((ascii_to_int(temp_data[0]) + ascii_to_int(temp_data[2]) + ascii_to_int(temp_data[3]))%2 == 0) ? '0' : '1';
  char c3 = ((ascii_to_int(temp_data[1]) + ascii_to_int(temp_data[2]) + ascii_to_int(temp_data[3]))%2 == 0) ? '0' : '1';

  *corrected = c1;
  *++corrected = c2;
  *++corrected = temp_data[0];
  *++corrected = c3;
  *++corrected = temp_data[1];
  *++corrected = temp_data[2];
  *++corrected = temp_data[3];

  //move on
  add_error_correction(data, corrected+1);  

}


int binary_to_pos(const char* binary){
  int pos = 0;
  for (int i=0; i<static_cast<int>(std::strlen(binary)); i++){
    pos += ascii_to_int(binary[i])*(std::exp2(static_cast<int>(std::strlen(binary)) - i - 1));
  }

  return pos;
}


void decode_recursive(const char* received, char* decoded, int& count){
  if (*received == '\0'){
    return;
  }
  //extract received bits in batches of 7
  char temp_data[8] = {};
  for (int i=0; i<7; i++){
    temp_data[i] = *received;
    received++;
  }

  //compute check bits
  char p1 = ((ascii_to_int(temp_data[3]) + ascii_to_int(temp_data[4]) + ascii_to_int(temp_data[5]) + ascii_to_int(temp_data[6]))%2 == 0) ? '0' : '1';
  char p2 = ((ascii_to_int(temp_data[1]) + ascii_to_int(temp_data[2]) + ascii_to_int(temp_data[5]) + ascii_to_int(temp_data[6]))%2 == 0) ? '0' : '1';
  char p3 = ((ascii_to_int(temp_data[0]) + ascii_to_int(temp_data[2]) + ascii_to_int(temp_data[4]) + ascii_to_int(temp_data[6]))%2 == 0) ? '0' : '1';

  if ((ascii_to_int(p1)+ascii_to_int(p2)+ascii_to_int(p3)) != 0){
    // get bad bit position
    char bad_bit_binary[4] = {p1, p2, p3};
    int bad_bit_pos = binary_to_pos(bad_bit_binary);

    // flip the bad bit
    temp_data[bad_bit_pos-1] = (temp_data[bad_bit_pos-1] == '0') ? '1' : '0';

    //increment our error count
    count++;
 
  }
  *decoded = temp_data[2];
  *++decoded = temp_data[4];
  *++decoded = temp_data[5];
  *++decoded = temp_data[6];

  //move on
  decode_recursive(received, decoded+1, count);

}


int decode(const char* received, char* decoded){
  int count = 0;
  decode_recursive(received, decoded, count);
  return count;
}


