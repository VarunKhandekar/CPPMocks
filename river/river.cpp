#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
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

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */

char** make_river_scene(const char* left, const char* boat){
  char** scene = create_scene();
  //left: (0,0). bank goes up to 18. total width is 72
  //right: (0,53)
  //river: (19,19)

  // add banks river and sun
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 19, 19, "river.txt");
  add_to_scene(scene, 3, 30, "sun.txt");


  // add left
  int m_count_left = 0;
  int c_count_left = 0;
  int m_index_left = 1;
  int c_index_left = 1;
  bool boat_on_left = false;
  while (*left != '\0'){
    if (*left == 'M'){
      // missionary row start is 2
      add_to_scene(scene, 2, m_index_left, "missionary.txt");
      // missionary is 5 wide. We want a space after
      m_index_left += 6;
      m_count_left++;
    }
    else if (*left == 'C'){
      // cannibal row start is 11
      add_to_scene(scene, 11, c_index_left, "cannibal.txt");
      // cannibal is 5 wide. We want a space after
      c_index_left += 6;
      c_count_left++;
    }
   
    else if (*left == 'B'){
      boat_on_left = true;
    }
    left++;
  }

  // add the boat. boat is 2 tall and 17 wide
  // starts at row 17. Entities start 3 into the boat
  int index_boat = 22;
  if (boat_on_left){
    add_to_scene(scene, 17, 19, "boat.txt");
  }
  else {
    add_to_scene(scene, 17, 36, "boat.txt");
    index_boat = 39;
  }
  // add boat entities
  int m_count_boat = 0;
  int c_count_boat = 0;

  while (*boat != '\0'){
    if (*boat == 'M'){
      // boat row start is 2
      add_to_scene(scene, 11, index_boat, "missionary.txt");
      // missionary is 5 wide. We want a space after
      index_boat += 6;
      m_count_boat++;
    }
    else if (*boat == 'C'){
      // boat row start is 11
      add_to_scene(scene, 11, index_boat, "cannibal.txt");
      // cannibal is 5 wide. We want a space after
      index_boat += 6;
      c_count_boat++;
    }
    boat++;
  }

  // add right
  int m_right = 3 - m_count_left - m_count_boat;
  int c_right = 3 - c_count_left - c_count_boat;
  int m_index_right = 54;
  int c_index_right = 54;
  // add missionaries
  for (int i=0; i < m_right; i++){
    add_to_scene(scene, 2, m_index_right, "missionary.txt");
    m_index_right += 6; 
  }
  // add cannibals
  for (int i=0; i < c_right; i++){
    add_to_scene(scene, 11, c_index_right, "cannibal.txt");
    c_index_right += 6;
  }

  return scene;
}

Status perform_crossing(char* left, char* targets){
  /* Error checking */
  //Status result;
  if (std::strlen(targets) > 2){
    return ERROR_INVALID_MOVE;
  }

  char** scene = create_scene();

  /* 1) loading the boat */
  int m_count = 0;
  int c_count = 0;
  //char new_left[8] = {};
  bool boat_on_left = false;

  for (int i=0; left[i] != '\0'; i++){
    if (left[i] == 'M'){
      m_count++;
    }
    else if (left[i] == 'C'){
      c_count++;
    }
    else if (left[i] == 'B'){
      boat_on_left = true;
    }
  }
  // check what we are loading the boat with
  int m_count_target = 0;
  int c_count_target = 0;
  for (int i=0; i<2; i++){
    if (targets[i] == 'M'){
      m_count_target++;
    }
    else if (targets[i] == 'C'){
      c_count_target++;
    }
  }
  
  // error checking
  if ((m_count > 3)){
    return ERROR_INVALID_MISSIONARY_COUNT;
  }
  if ((c_count > 3)){
    return ERROR_INVALID_CANNIBAL_COUNT;
  }

  // edit what we want on the left
  if (boat_on_left){
    if (m_count_target>m_count){
      return ERROR_INVALID_MOVE;
    }
    if (c_count_target>c_count){
      return ERROR_INVALID_MOVE;
    }
    m_count = m_count - m_count_target;
    c_count = c_count - c_count_target;
  }
  else {
    if (m_count_target+m_count > 3){
      return ERROR_INVALID_MOVE;
    }
    if (c_count_target+c_count >3){
      return ERROR_INVALID_MOVE;
    }
    m_count = m_count + m_count_target;
    c_count = c_count + c_count_target;
  }

  // check we have not exceeded the total number of missionaries or cannibals
  //if ((m_count+m_count_target > 3)){
  //  return ERROR_INVALID_MISSIONARY_COUNT;
  //}
  //if ((c_count+c_count_target > 3)){
  //  return ERROR_INVALID_CANNIBAL_COUNT;
  //}

  // generate new string for the left
  int new_left_index = 0;
  // add missionaries
  for (int i = 0; i<m_count; i++){
    left[new_left_index] = 'M';
    new_left_index++;
  }
  // add cannibals
  for (int i = 0; i<c_count; i++){
    left[new_left_index] = 'C';
    new_left_index++;
  }
  // add boat
  if (boat_on_left){
    left[new_left_index] = 'B';
    new_left_index++;
  }
  left[new_left_index] = '\0';
  scene = make_river_scene(left, targets);
  print_scene(scene);
  destroy_scene(scene);

  /* 2) Crossing the river */
  //memset(left, 0, strlen(left));
  // generate new string for the left
  new_left_index = 0;
  // add missionaries
  for (int i = 0; i<m_count; i++){
    left[new_left_index] = 'M';
    new_left_index++;
  }
  // add cannibals
  for (int i = 0; i<c_count; i++){
    left[new_left_index] = 'C';
    new_left_index++;
  }
  // add boat if required
  if (!boat_on_left){
    left[new_left_index] = 'B';
    new_left_index++;
    
  }
  boat_on_left = !boat_on_left;
  left[new_left_index] = '\0';

  scene = make_river_scene(left, targets);
  print_scene(scene);
  destroy_scene(scene);

  /* 3) Unloading the boat */
  scene = make_river_scene(left, "");
  print_scene(scene);
  destroy_scene(scene);

  // missionaries eaten
  // right then left
  if (boat_on_left){
    if ((3-c_count)>(3-m_count) && (3-m_count)!=0){
      return ERROR_MISSIONARIES_EATEN;
    }
  }
  else {
    if (c_count > m_count && m_count != 0){
      cout << c_count << endl;
      return ERROR_MISSIONARIES_EATEN;
    }
  }

  // we have not got everyone over from the left
  if (strlen(left) != 0){
    return VALID_NONGOAL_STATE;
  }

  return VALID_GOAL_STATE;
}

Status play_game(){
  // initialise game
  char left[8] = "BCCCMMM";
  Status result = VALID_NONGOAL_STATE;
  char boat_crossing[3] = "";
  //Status result;
  char** scene = make_river_scene(left,"");
  print_scene(scene);
  destroy_scene(scene);

  // begin play
  do
  {
    //result = perform_crossing(left, boat_crossing);
    cout << "Please suggest a one or two letter (M or C) boat crossing via the keyboard: " << endl;
    cin >> boat_crossing;
    if (strlen(boat_crossing)==0){
      continue;
    }

    for (int i=0; boat_crossing[i]!='\0'; i++){
      boat_crossing[i] = toupper(boat_crossing[i]);
    }
    result = perform_crossing(left, boat_crossing);
    cout << left << endl;
    // perform requisite status checks
    // game is over for some reason
    if (result != VALID_NONGOAL_STATE){
      return result;
    }
  }
  while (result == VALID_NONGOAL_STATE);

  return VALID_NONGOAL_STATE;
}


Status find_solution(char* left, char* answer){

  return VALID_GOAL_STATE;
}