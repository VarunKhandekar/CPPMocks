#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}



bool get_symbol_position(char** map, int height, int width, char target, int& r, int& c){
  r = -1;
  c = -1;
  for (int row = 0; row < height; row++){
    for (int col = 0; col < width; col++){
      if (map[row][col] == target){
        r = row;
        c = col;
        return true;
      }
    }
  }
  return false;
}


char get_code_helper(const char* name, const char* filename){
  ifstream in(filename);
  char code;
  char line_or_station[512] = {};
  while (!in.eof()){
    in >> code;
    int i = 0;
    char ch;
    // we are pointing at a space. Skip over that i.e. get a ch twice
    in.get(ch);
    in.get(ch);
    while (ch != '\n'){
      line_or_station[i] = ch;
      i++;
      in.get(ch);
    }
    line_or_station[i] = '\0';
    if (strcmp(name, line_or_station) == 0){
      in.close();
      return code;
    }  
  }
  in.close();
  return ' ';
}


char get_symbol_for_station_or_line(const char* name){
  ifstream in("stations.txt");
  char code = ' ';
  code = get_code_helper(name, "stations.txt");
  if (code != ' '){
    return code;
  }
  else {
    code = get_code_helper(name, "lines.txt");
  }
  return code;
}


void direction_to_deltas(Direction d, int& r_delta, int& c_delta){
  switch(d){
    case N: r_delta = -1;
            c_delta = 0;
            return;
    case S: r_delta = 1;
            c_delta = 0;
            return;
    case W: r_delta = 0;
            c_delta = -1;
            return;
    case E: r_delta = 0;
            c_delta = 1;
            return;
    case NE: r_delta = -1;
             c_delta = 1;
             return;
    case NW: r_delta = -1;
             c_delta = -1;
             return;
    case SE: r_delta = 1;
             c_delta = 1;
             return;
    case SW: r_delta = 1;
             c_delta = -1;
             return;
    default: r_delta = 0;
             c_delta = 0;
             return;
  }
}

Direction get_reverse_dir(Direction d){
  switch(d){
    case N: return S;
    case S: return N;
    case W: return E;
    case E: return W;
    case NE: return SW;
    case NW: return SE;
    case SE: return NW;
    case SW: return NE;
    default: return INVALID_DIRECTION;
  }
}

bool is_station(char** map, int row, int col){
  char ch = map[row][col];
  if (isalnum(ch) == 0){
    return false;
  }
  return true;
}


void get_station_name(char ch, char* destination) {
  ifstream in("stations.txt");
  char code;
  char station[512] = {};
  while (!in.eof()){
    in >> code;
    int i = 0;
    char c;
    // we are pointing at a space. Skip over that i.e. get a ch twice
    in.get(c);
    in.get(c);
    while (c != '\n'){
      station[i] = c;
      i++;
      in.get(c);
    }
    station[i] = '\0';
    if (ch == code){
      strcpy(destination, station);
      return;
    }  
  }
  in.close();
}


int validate_route(char** map, int height, int width, const char* start_station, char* route, char* destination){
  // validate start station
  char start_symbol = get_symbol_for_station_or_line(start_station);
  if (start_symbol == ' '){
    return ERROR_START_STATION_INVALID;
  }
  // validate directions
  char route_copy[512];
  strcpy(route_copy, route);
  char* token;
  token = strtok(route_copy, ",");
  int prev_dir = -1;
  while (token != nullptr){
    if (strlen(token) > 2){
      return ERROR_INVALID_DIRECTION;
    }
    // check letters
    Direction dir = string_to_direction(token);
    if (dir == ERROR_INVALID_DIRECTION){
      return ERROR_INVALID_DIRECTION;
    }
    if (get_reverse_dir(dir) == prev_dir){
      return ERROR_BACKTRACKING_BETWEEN_STATIONS;
    }
    prev_dir = dir;
    token = strtok(nullptr, ",");
  }

  // follow journey
  int r, c;
  get_symbol_position(map, height, width, start_symbol, r, c);
  token = strtok(route, ",");
  while (token != nullptr){
    // get current char
    char old_ch = map[r][c];
    // get our new map position
    Direction dir = string_to_direction(token);
    int r_delta, c_delta;
    direction_to_deltas(dir, r_delta, c_delta);
    r += r_delta;
    c += c_delta;
    // check bounds
    if (r < 0 || r > (height-1) || c < 0 || c > (width -1)){
      return ERROR_OUT_OF_BOUNDS;
    }

    // check for line hopping
    if (isalnum(map[r][c])==0 && isalnum(old_ch) == 0 && map[r][c]!=old_ch){
      return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
    }


    token = strtok(nullptr, ",");
  }

  
  // we are at the end of the route. Check that where we are is a station
  if (!is_station(map, r, c)){
    cout << map[r][c] << endl;
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  }

  // route was all ok. Get station name
  get_station_name(map[r][c], destination);
  return 0;
}