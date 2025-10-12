#include "battle_message.h"

char* get_action_str(int type){
    switch (type){
        case 0:
            return "Laser Attack";
        case 1:
            return "Photon Torpedo";
        case 2:
            return "Shields Up";
        case 3:
            return "Cloaking";
        case 4:
            return "Hyper Jump";
        default:
            return "Selecione uma ação válida";
    }
}

char* get_client_action_message(BattleMessage* msg){
    return get_action_str(msg->client_action);
}

char* get_server_action_message(BattleMessage* msg){
    return get_action_str(msg->server_action);
}

void get_score(char* buf, int client_hp, int server_hp){
    snprintf(buf, 32, "Você %d x %d Inimigo\n", client_hp, server_hp);
}

void get_message(BattleMessage* msg){
    switch (msg->type) {
        case MSG_INIT:
            snprintf(msg->message, MSG_SIZE, "%s", "Conectado ao servidor.\nSua nave: SS-42 Voyager (HP: 100)\n");
            break;
        case MSG_ACTION_REQ:
            snprintf(msg->message, MSG_SIZE,  "%s",
            "Escolha sua ação:\n0 - Laser Attack\n1 - Photon Torpedo\n2 - Shields Up\n3 - Cloaking\n4 - Hyper Jump\n"); 
            break;
        case MSG_ACTION_RES:
            snprintf(msg->message, MSG_SIZE, "Você usou %s.\nServidor usou %s.\n", get_client_action_message(msg), get_server_action_message(msg));
            break;
        case MSG_BATTLE_RESULT:
            char buf[32];
            get_score(buf, msg->client_hp, msg->server_hp);
            snprintf(msg->message, MSG_SIZE, "Resultado: %s\n%s\n", combinations[msg->client_action][msg->server_action].msg, buf);
            break;
        case MSG_INVENTORY:
            snprintf(msg->message, MSG_SIZE, 
            "Inventário final: \n- HP restante: %d\n- Torpedos usados: %d\n- Escudos usados: %d\n", msg->client_hp, msg->client_torpedoes, msg->client_shields);
            break;
        case MSG_GAME_OVER:
            if(msg->client_hp <= 0 && msg->server_hp <= 0){
                snprintf(msg->message, MSG_SIZE, "%s", "Fim de jogo. Ambos foram destruídos!\nObrigado por jogar!\n");
            } else if(msg->client_hp <= 0){
                snprintf(msg->message, MSG_SIZE, "%s", "Fim de jogo. Sua nave foi destruída!\nObrigado por jogar!\n");
            } else if(msg->server_hp <= 0){
                snprintf(msg->message, MSG_SIZE, "%s", "Fim de jogo. Você destruiu a frota inimiga!\nObrigado por jogar!\n");
            } else {
                snprintf(msg->message, MSG_SIZE, "%s", "Fim de jogo!\n");
            }
            break;
        case MSG_ESCAPE:
            snprintf(msg->message, MSG_SIZE, "%s", combinations[msg->client_action][msg->server_action].msg);
            break;
        default:
            snprintf(msg->message, MSG_SIZE, "%s", "Selecione uma mensagem válida\n");
            break;
    }
}