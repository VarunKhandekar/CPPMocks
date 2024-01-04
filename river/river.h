#ifndef RIVER_H
#define RIVER_H
const int SCENE_HEIGHT=24, SCENE_WIDTH=72;

enum Status {VALID_GOAL_STATE=2, VALID_NONGOAL_STATE=1,ERROR_INVALID_CANNIBAL_COUNT=-1, ERROR_INVALID_MISSIONARY_COUNT=-2, ERROR_INVALID_MOVE=-3, ERROR_MISSIONARIES_EATEN=-4, ERROR_INPUT_STREAM_ERROR=-5, ERROR_BONUS_NO_SOLUTION=-6};

char **allocate_2D_array(int, int);

void deallocate_2D_array(char**, int);

char **create_scene();

void destroy_scene(char**);

void print_scene(char**);

void filter(char*);

bool add_to_scene(char**, int, int, const char*);

const char* status_description(int);




char** make_river_scene(const char*, const char*);

Status perform_crossing(char*, char*);

Status play_game();

Status find_solution(char*, char*);

#endif