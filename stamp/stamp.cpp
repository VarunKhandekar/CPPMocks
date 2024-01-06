#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"
#include <cctype>
#include <string>
#include <cstdlib>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */


int leading_zeros(const char* digest){
  int count = 0;
  int i = 0;
  while (digest[i] != '\0'){
    if (digest[i] == '0'){
      count++;
    }
    else {
      break;
    }
    i++;
  }

  i = 0;
  while (digest[i] != '\0'){
    if (isxdigit(digest[i]) == 0){
      return -1;
    }
    i++;
  }
  return count;
}


bool file_to_SHA1_digest(const char* filename, char* digest){
  ifstream in(filename);
  if (in.fail()){
    return false;
  }
  char text[50000];
  int i = 0;
  char ch;
  in.get(ch);
  while (!in.eof()){
    text[i] = ch;
    i++;
    in.get(ch);
  }
  text_to_SHA1_digest(text, digest);

  in.close();
  return true;
}


void int_to_str(int i, char* str){


}


bool make_header(const char* recipient, const char* filename, char* header){
  memset(header, 0, strlen(header));
  char initial_digest[41];
  if (!file_to_SHA1_digest(filename, initial_digest)){
    return false;
  }

  // prepare initial header
  strcat(header, recipient);
  strcat(header, ":");
  strcat(header, initial_digest);
  strcat(header, ":");
  strcat(header, "0");

  // prepare next header
  int counter = 0;
  char digest[41];
  text_to_SHA1_digest(header, digest);
  while (leading_zeros(digest) != 5){
    memset(header, 0, strlen(header));
    counter++;
    if (counter > 10000000){
      return false;
    }
    char counter_str[9];
    sprintf(counter_str, "%d", counter);
    strcat(header, recipient);
    strcat(header, ":");
    strcat(header, initial_digest);
    strcat(header, ":");
    strcat(header, counter_str);
    text_to_SHA1_digest(header, digest);
  }

  return true;
}

MessageStatus check_header(const char* email_address, const char* header, const char* filename){
  // check header is in correct form
  int count = 0;
  char header_copy[512];
  strcpy(header_copy, header);
  char* token;
  token = strtok(header_copy, ":");
  while (token != nullptr){
    count++;
    token = strtok(nullptr, ":");
  }
  if (count != 3){
    return INVALID_HEADER;
  }

  // check email matches up
  memset(header_copy, 0, strlen(header_copy));
  strcpy(header_copy, header);
  token = strtok(header_copy, ":");
  if (strcmp(token, email_address) != 0){
    return WRONG_RECIPIENT;
  }

  // check SHA1 digest in the header matches the SHA1 digest of the received message
  char received_message_digest[41];
  file_to_SHA1_digest(filename, received_message_digest);
  token = strtok(nullptr, ":");
  if (strcmp(token, received_message_digest) != 0){
    return INVALID_MESSAGE_DIGEST;
  }

  // check SHA1 digest of the header is an acceptable header
  char header_digest[41];
  text_to_SHA1_digest(header, header_digest);
  if (leading_zeros(header_digest) != 5){
    return INVALID_HEADER_DIGEST;
  }

  return VALID_EMAIL;
}
