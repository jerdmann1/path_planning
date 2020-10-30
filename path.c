#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h> 
#include <stdlib.h>
#include <unistd.h>

#define MAX_OBSTACLES 25 /* maximum number of obstacles */
#define NUM_WALLS 54

typedef struct {
    int x;
    int y;
} Element;

bool is_unseen(char c){
    return c == '_';
}

void print_room(char room[17][11]){
    printf("\x1b[2J");
    for(int i = 0; i < 17; i++){
        for(int j = 0; j < 11; j++){
            if(room[i][j] == 0){
                printf("%5c", 'G');
            }
            else if(room[i][j] != 'X' && room[i][j] != 'S' && room[i][j] != '_'){
                printf("%5d", room[i][j] - '0');
            }else{
                printf("%5c", room[i][j]);
            }
        }
        printf("\n");
    }
}

void bfs(char room[17][11], int goal[2]){
    Element q[1700];
    int cnt = 0, orig_cnt = 0, prev_orig_cnt = 0, step = 1, u = 0;
    Element temp = {goal[0], goal[1]};
    q[orig_cnt++] = temp;
    while((orig_cnt - prev_orig_cnt) > 0){
        print_room(room);
        sleep(1);
        for(int i = prev_orig_cnt; i < orig_cnt; i++){
            if(q[i].x - 1 >= 0){
                if(is_unseen(room[q[i].x - 1][q[i].y])){
                    room[q[i].x - 1][q[i].y] = step + '0';
                    Element temp = {q[i].x - 1, q[i].y};
                    q[orig_cnt + cnt++] = temp;
                    u++;
                }
            }
            if(q[i].x + 1 < 17){
                if(is_unseen(room[q[i].x + 1][q[i].y])){
                    room[q[i].x + 1][q[i].y] = step + '0';
                    Element temp = {q[i].x + 1, q[i].y};
                    q[orig_cnt + cnt++] = temp;
                    u++;
                }
            }
            if(q[i].y - 1 >= 0){
                if(is_unseen(room[q[i].x][q[i].y - 1])){
                    room[q[i].x][q[i].y - 1] = step + '0';
                    Element temp = {q[i].x, q[i].y - 1};
                    q[orig_cnt + cnt++] = temp;
                    u++;
                }
            }
            if(q[i].y + 1 < 11){
                if(is_unseen(room[q[i].x][q[i].y + 1])){
                    room[q[i].x][q[i].y + 1] = step + '0';
                    Element temp = {q[i].x, q[i].y + 1};
                    q[orig_cnt + cnt++] = temp;
                    u++;
                }
            }
        }
        step++;
        cnt = 0;
        prev_orig_cnt = orig_cnt;
        orig_cnt += u;
        u = 0;
    }
}

void get_commands(int commands[50], char room[17][11], int goal[2], int start[2]){
    int curr[2] = {start[0], start[1]};
    int i = 0, orientation = 1;
    while(goal[0] != curr[0] || goal[1] != curr[1]){
        int min = INT_MAX;
        int best_pos[2];
        int comm = 0;
        if(curr[0] - 1 >= 0){
            if(room[curr[0]-1][curr[1]] != 'X'){
                if(room[curr[0]-1][curr[1]] < min){
                    min = room[curr[0]-1][curr[1]];
                    best_pos[0] = curr[0]-1;
                    best_pos[1] = curr[1];
                    comm = -1;
                }
            }
        }
        if(curr[0] + 1 < 17){
            if(room[curr[0]+1][curr[1]] != 'X'){
                if(room[curr[0]+1][curr[1]] < min){
                    min = room[curr[0]+1][curr[1]];
                    best_pos[0] = curr[0]+1;
                    best_pos[1] = curr[1];
                    comm = 1;
                }
            }
        }
        if(curr[1] - 1 >= 0){
            if(room[curr[0]][curr[1]-1] != 'X'){
                if(room[curr[0]][curr[1]-1] < min){
                    min = room[curr[0]][curr[1]-1];
                    best_pos[0] = curr[0];
                    best_pos[1] = curr[1]-1;
                    comm = 2;
                }
            }
        }
        if(curr[1] + 1 < 11){
            if(room[curr[0]][curr[1]+1] != 'X'){
                if(room[curr[0]][curr[1]+1] < min){
                    min = room[curr[0]][curr[1]+1];
                    best_pos[0] = curr[0];
                    best_pos[1] = curr[1]+1;
                    comm = -2;
                }
            }
        }
        curr[0] = best_pos[0];
        curr[1] = best_pos[1];
        if(comm != orientation){
            if(comm == 2 && orientation == 1){
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = 2;
            }else if(comm == 1 && orientation == 2){
                commands[i++] = -2;
                commands[i++] = 1;
                orientation = 1;
            }else if(comm == 1 && orientation == -2){
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = 1;
            }else if(comm == -2 && orientation == 1){
                commands[i++] = -2;
                commands[i++] = 1;
                orientation = -2;
            }else if(comm == -2 && orientation == -1){
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = -2;
            }else if(comm == 2 && orientation == -1){
                commands[i++] = -2;
                commands[i++] = 1;
                orientation = 2;
            }else if(comm == -1 && orientation == 2){
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = -1;
            }else if(comm == -1 && orientation == -2){
                commands[i++] = -2;
                commands[i++] = 1;
                orientation = -1;
            }else if(comm == -1 && orientation == 1){
                commands[i++] = 2;
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = -1;
            }else if(comm == 1 && orientation == -1){
                commands[i++] = 2;
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = 1;
            }else if(comm == 2 && orientation == -2){
                commands[i++] = 2;
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = 2;
            }else if(comm == -2 && orientation == 2){
                commands[i++] = 2;
                commands[i++] = 2;
                commands[i++] = 1;
                orientation = -2;
            }
        }else{
            commands[i++] = 1;
        } 
    }
}

/*
int num_obstacles = 13;
double obstacle[MAX_OBSTACLES][2] = 
{{0.61, 2.743},{0.915, 2.743},{1.219, 2.743},{1.829, 1.219},
{1.829, 1.524},{ 1.829, 1.829}, {1.829, 2.134},{2.743, 0.305},
{2.743, 0.61},{2.743, 0.915},{2.743, 2.743},{3.048, 2.743},
{3.353, 2.743}};

double start[2] = {0.305, 1.219};

double goal[2] = {3.658, 1.829};
*/

/*
int num_obstacles = 13;
double obstacle[MAX_OBSTACLES][2] = 
{{0.61, 2.743},{0.915, 2.743},{1.219, 2.743},{1.829, 1.219},
{1.829, 1.524},{ 1.829, 1.829}, {1.829, 2.134},{2.743, 0.305},
{2.743, 0.61},{2.743, 0.915},{2.743, 2.743},{3.048, 2.743},
{3.353, 2.743}};

double goal[2] = {0.305, 1.219};

double start[2] = {3.658, 1.829};
*/

/*
int num_obstacles = 19;

double obstacle[MAX_OBSTACLES][2] =
{{3*0.305,3*0.305},{3*0.305,4*0.305},{3*0.305,5*0.305},{3*0.305,6*0.305},{3*0.305,7*0.305},{7*0.305,5*0.305},
{7*0.305,6*0.305},{7*0.305,7*0.305},{7*0.305,8*0.305},{7*0.305,9*0.305},{7*0.305,9*0.305},{11*0.305,2*0.305},
{11*0.305,0*0.305},{11*0.305,1*0.305},{11*0.305,3*0.305},{11*0.305,4*0.305},{11*0.305,5*0.305},{11*0.305,6*0.305},
{11*0.305,7*0.305}};

double start[2] = {0.305, 5*0.305}; 

double goal[2] = {15*0.305, 5*0.305}; 
*/

/*
int num_obstacles = 19;

double obstacle[MAX_OBSTACLES][2] =
{{3*0.305,3*0.305},{3*0.305,4*0.305},{3*0.305,5*0.305},{3*0.305,6*0.305},{3*0.305,7*0.305},{7*0.305,5*0.305},
{7*0.305,6*0.305},{7*0.305,7*0.305},{7*0.305,8*0.305},{7*0.305,9*0.305},{7*0.305,9*0.305},{11*0.305,2*0.305},
{11*0.305,0*0.305},{11*0.305,1*0.305},{11*0.305,3*0.305},{11*0.305,4*0.305},{11*0.305,5*0.305},{11*0.305,6*0.305},
{11*0.305,7*0.305}};

double goal[2] = {0.305, 5*0.305}; 

double start[2] = {15*0.305, 5*0.305}; 
*/

/*
int num_obstacles = 12;

double obstacle[MAX_OBSTACLES][2] =
{{4*0.305, 3*0.305},{5*0.305, 3*0.305},{6*0.305, 3*0.305},{7*0.305, 3*0.305},{8*0.305, 3*0.305},{9*0.305, 3*0.305},
{7*0.305, 6*0.305},{8*0.305, 6*0.305},{9*0.305, 6*0.305},{10*0.305, 6*0.305},{11*0.305, 6*0.305},{12*0.305, 6*0.305}};

double start[2] = {10*0.305, 1*0.305}; 

double goal[2] = {3*0.305, 9*0.305}; 
*/


int num_obstacles = 12;

double obstacle[MAX_OBSTACLES][2] =
{{4*0.305, 3*0.305},{5*0.305, 3*0.305},{6*0.305, 3*0.305},{7*0.305, 3*0.305},{8*0.305, 3*0.305},{9*0.305, 3*0.305},
{7*0.305, 6*0.305},{8*0.305, 6*0.305},{9*0.305, 6*0.305},{10*0.305, 6*0.305},{11*0.305, 6*0.305},{12*0.305, 6*0.305}};

double start[2] = {10*0.305, 1*0.305}; 

double goal[2] = {3*0.305, 9*0.305}; 

/*
int num_obstacles = 12;

double obstacle[MAX_OBSTACLES][2] =
{{4*0.305, 3*0.305},{5*0.305, 3*0.305},{6*0.305, 3*0.305},{7*0.305, 3*0.305},{8*0.305, 3*0.305},{9*0.305, 3*0.305},
{7*0.305, 6*0.305},{8*0.305, 6*0.305},{9*0.305, 6*0.305},{10*0.305, 6*0.305},{11*0.305, 6*0.305},{12*0.305, 6*0.305},};

double goal[2] = {10*0.305, 1*0.305}; 

double start[2] = {3*0.305, 9*0.305}; 
*/
bool check_box(int i, int j){
    for(int k = 0; k < num_obstacles; k++){
        if((round(obstacle[k][0] / 0.305) == i) && (round(obstacle[k][1] / 0.305) == j)){
            return true;
        }
    }
    return false;
}


double walls[NUM_WALLS][2] =
{{0*0.305, 0*.305},{0*0.305, 1*.305},{0*0.305, 2*.305},{0*0.305, 3*.305},{0*0.305, 4*.305},{0*0.305, 5*.305},
{0*0.305, 6*.305},{0*0.305, 7*.305},{0*0.305, 8*.305},{0*0.305, 9*.305},{0*0.305, 10*.305},{16*0.305, 0*.305},
{16*0.305, 1*.305},{16*0.305, 2*.305},{16*0.305, 3*.305},{16*0.305, 4*.305},{16*0.305, 5*.305},
{16*0.305, 6*.305},{16*0.305, 7*.305},{16*0.305, 8*.305},{16*0.305, 9*.305},{16*0.305, 10*.305},
{1*0.305,0},{2*0.305, 0},{3*0.305, 0},{4*0.305, 0},{5*0.305, 0},{6*0.305, 0},{7*0.305, 0},{8*0.305, 0},
{9*0.305, 0},{10*0.305, 0},{11*0.305, 0},{12*0.305, 0},{13*0.305, 0},{14*0.305, 0},{15*0.305, 0},{16*0.305, 0},
{1*0.305,10*0.305},{2*0.305, 10*0.305},{3*0.305, 10*0.305},{4*0.305, 10*0.305},{5*0.305, 10*0.305},{6*0.305,10*0.305},
{7*0.305, 10*0.305},{8*0.305, 10*0.305},{9*0.305, 10*0.305},{10*0.305, 10*0.305},{11*0.305, 10*0.305},
{12*0.305, 10*0.305},{13*0.305, 10*0.305},{14*0.305, 10*0.305},{15*0.305, 10*0.305},{16*0.305, 10*0.305}};

int main(){
    char room[17][11];
    int commands[50];
    int i = 0;
    int j = 0; 
    for(int i = 0; i < 50; i++){
        commands[i] = 0;
    }
    for(int i = 0; i < NUM_WALLS; i++){
        int x = round(walls[i][0] / 0.305);
        int y = round(walls[i][1] / 0.305);
        room[x][y] = 'X';
    }
    for(int i = 1; i < 16; i++){
        for(int j = 1; j < 10; j++){
            if(round(start[0] / 0.305) == i && round(start[1] / 0.305) == j){
                room[i][j] = 'S';
            }else if(round(goal[0] / 0.305) == i && round(goal[1] / 0.305) == j){
                room[i][j] = 0;
            }
            else if(check_box(i, j)){
                room[i][j] = 'X';
            }else{
                room[i][j] = '_';
            }
        }
    }
    int goal_cords[2] = {(int)round(goal[0] / 0.305), (int)round(goal[1] / 0.305)};
    int start_cords[2] = {(int)round(start[0] / 0.305), (int)round(start[1] / 0.305)};
    bfs(room, goal_cords);
    get_commands(commands, room, goal_cords, start_cords);
    printf("\n");
    while(commands[i] != 0){
        printf("%3d", commands[i++]);
    }
    printf("\n");
    return 0;
}