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
    snprintf(buf, sizeof(buf), "Placar: Você: %d x %d Inimigo", client_hp, server_hp);
}

void get_message(BattleMessage* msg){
    switch (msg->type) {
        case MSG_INIT:
            snprintf(msg->message, MSG_SIZE, "Conectado ao servidor.\n Sua nave: SS-42 Voyager (HP: 100)");
            break;
        case MSG_ACTION_REQ:
            snprintf(msg->message, MSG_SIZE, 
            "Escolha sua ação:\n\
            0 - Laser Attack\n\
            1 - Photon Torpedo\n\
            2 - Shields Up\n\
            3 - Cloaking\n\
            4 - Hyper Jump\n"); 
            break;
        case MSG_ACTION_RES:
            snprintf(msg->message, MSG_SIZE, "Você usou %s.\nServidor usou %s\n.", get_client_action_message(msg), get_server_action_message(msg));
            break;
        case MSG_BATTLE_RESULT:
            char* buf[32];
            get_score(buf, msg->client_hp, msg->server_hp);
            snprintf(msg->message, MSG_SIZE, "Resultado: \n%s\n%s", combinations[msg->client_action][msg->server_action].msg, buf);
            break;
        case MSG_INVENTORY:
            snprintf(msg->message, MSG_SIZE, "Fim de jogo!\n\
            Inventário final: \n\
            - HP restante: %d\n\
            - Torpedos usado: %d\n\
            - Escudos usados: %d\n", msg->client_hp, msg->client_torpedoes, msg->client_shields);
            break;
        case MSG_GAME_OVER:
            if(msg->client_hp <= 0 && msg->server_hp <= 0){
                snprintf(msg->message, MSG_SIZE, "Ambos foram destruídos!\nObrigado por jogar!\n");
            } else if(msg->client_hp <= 0){
                snprintf(msg->message, MSG_SIZE, "Sua nave foi destruída!\nObrigado por jogar!\n");
            } else if(msg->server_hp <= 0){
                snprintf(msg->message, MSG_SIZE, "Você destruiu a frota inimiga!\nObrigado por jogar!\n");
            }
            break;
        case MSG_ESCAPE:
            snprintf(msg->message, MSG_SIZE, combinations[msg->client_action][msg->server_action].msg);
            break;
        default:
            snprintf(msg->message, MSG_SIZE, "Selecione uma mensagem válida");
            break;
    }
}