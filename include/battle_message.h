#ifndef BATTLE_MESSAGE_H
#define BATTLE_MESSAGE_H

#define MSG_SIZE 256

#include <stdio.h>
#include <stdint.h>
#include "actions_combination.h"

typedef enum {
    MSG_INIT,          // Boas-vindas
    MSG_ACTION_REQ,    // Solicitação de ação
    MSG_ACTION_RES,    // Cliente responde com ação escolhida
    MSG_BATTLE_RESULT, // Resultado do turno
    MSG_INVENTORY,     // Atualização de inventário
    MSG_GAME_OVER,     // Fim de jogo
    MSG_ESCAPE         // Jogador fugiu
} MessageType;

typedef struct {
    uint32_t type;
    uint32_t client_action;
    uint32_t server_action;
    uint32_t client_hp;
    uint32_t server_hp;
    uint32_t client_torpedoes;
    uint32_t client_shields;
    char message[MSG_SIZE];
    uint32_t n_rounds;
} BattleMessage;

void update_message(BattleMessage* msg); // sets message to its value
char* get_action_str(uint32_t type); // returns action string
char* get_client_action_message(BattleMessage* msg); // gets client action string
char* get_server_action_message(BattleMessage* msg); // gets server action string
void get_score(char* buf, uint32_t client_hp, uint32_t server_hp);

#endif

