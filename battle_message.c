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

void get_message(BattleMessage* msg){
    switch (msg->type) {
        case MSG_INIT:
            snprintf(msg->message, sizeof(msg->message), "Conectado ao servidor.\n Sua nave: SS-42 Voyager (HP: 1 00)");
            break;
        case MSG_ACTION_REQ:
            snprintf(msg->message, sizeof(msg->message), 
            "Escolha sua ação:\n\
            0 - Laser Attack\n\
            1 - Photon Torpedo\n\
            2 - Shields Up\n\
            3 - Cloaking\n\
            4 - Hyper Jump\n"); 
            break;
        case MSG_ACTION_RES:
            snprintf(msg->message, sizeof(msg->message), "Você usou %s.\nServidor usou %s\n.", get_client_action_message(msg), get_server_action_message(msg));
            break;
        case MSG_BATTLE_RESULT:
            /* code */
            snprintf(msg->message, sizeof(msg->message), );
            break;
        case MSG_INVENTORY:
            /* code */
            snprintf(msg->message, sizeof(msg->message), );
            break;
        case MSG_GAME_OVER:
            /* code */
            snprintf(msg->message, sizeof(msg->message), );
            break;
        case MSG_ESCAPE:
            /* code */
            snprintf(msg->message, sizeof(msg->message), );
            break;
        default:
            snprintf(msg->message, sizeof(msg->message), "Selecione uma mensagem válida");
            break;
    }
}