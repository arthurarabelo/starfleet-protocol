#ifndef ACTIONS_COMBINATION_H
#define ACTIONS_COMBINATION_H

#define NUMBER_OF_ACTIONS 5
#define MAXMSGSIZE 64

#include <stdint.h>

typedef struct {
    uint32_t client_damage;
    uint32_t server_damage;
    char msg[MAXMSGSIZE];
} Combination;

extern Combination combinations[NUMBER_OF_ACTIONS][NUMBER_OF_ACTIONS];

#endif