#ifndef ACTIONS_COMBINATION_H
#define ACTIONS_COMBINATION_H

#define NUMBER_OF_ACTIONS 5
#define MAXMSGSIZE 64

#include <stdbool.h>

typedef struct {
    int client_damage;
    int server_damage;
    bool hyper_jump;
    char msg[MAXMSGSIZE];
} Combination;

extern Combination combinations[NUMBER_OF_ACTIONS][NUMBER_OF_ACTIONS];

#endif