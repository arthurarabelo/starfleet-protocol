#ifndef BATTLE_MESSAGE_H
#define BATTLE_MESSAGE_H

#define MSG_SIZE 256

#include <stdio.h>
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
    int type;
    int client_action;
    int server_action;
    int client_hp;
    int server_hp;
    int client_torpedoes;
    int client_shields;
    char message[MSG_SIZE];
} BattleMessage;

char* get_message(BattleMessage* msg);
char* get_action_str(int type);
char* get_client_action_message(BattleMessage* msg);
char* get_server_action_message(BattleMessage* msg);

#endif

