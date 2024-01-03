#ifndef CORRECT_H
#define CORRECT_H

/* Helper function: converts a character into a binary string representation */
void ascii_to_binary(char letter, char* output);

/* Helper function: converts a binary string representation into a character */
char binary_to_ascii(const char* binary);

/* A function to convert a string representing text to a binary encoding */
void text_to_binary(const char*, char*);

/* A function to convert a binary encoding to text */
void binary_to_text(const char*, char*);

/* Helper function: converts a ascii character representing a single digit integer to the integer equivalent e.g. '1' goes to 1 */
int ascii_to_int(char);

/* Helper function: converts a single digit integer to an ascii character representing that integer */
char int_to_ascii(int);

/* Helper function: converts a string representing binary to the appropriate integer */
int binary_to_pos(const char*);

/* A function to convert a data bit stream to an error corrected version of that stream */
void add_error_correction(const char*, char*);

/* A function to output an error corrected version of a received bit stream, returning the total number of errors in the stream*/
int decode(const char*, char*);

#endif
