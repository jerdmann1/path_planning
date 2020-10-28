#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h> 
#include <stdlib.h>
#include <unistd.h>

#define MAX_OBSTACLES 25 /* maximum number of obstacles */

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

int num_obstacles = 13; /* number of obstacles */

double obstacle[MAX_OBSTACLES][2] = /* obstacle locations */
{{0.61, 2.743},{0.915, 2.743},{1.219, 2.743},{1.829, 1.219},
{1.829, 1.524},{ 1.829, 1.829}, {1.829, 2.134},{2.743, 0.305},
{2.743, 0.61},{2.743, 0.915},{2.743, 2.743},{3.048, 2.743},
{3.353, 2.743}};

double start[2] = {0.305, 1.219}; /* start location */

double goal[2] = {3.658, 1.829}; /* goal location */

bool check_box(int i, int j){
    for(int k = 0; k < num_obstacles; k++){
        if((round(obstacle[k][0] / 0.305) == i) && (round(obstacle[k][1] / 0.305) == j)){
            return true;
        }
    }
    return false;
}

int main(){
    char room[17][11] = {'_'};
    int commands[50];
    int i = 0;
    for(int i = 0; i < 50; i++){
        commands[i] = 0;
    }
    for(int i = 0; i < 17; i++){
        for(int j = 0; j < 11; j++){
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