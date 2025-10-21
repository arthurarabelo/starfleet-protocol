#include "communication.h"

void host_to_network_long(BattleMessage *msg){
    msg->type = htonl(msg->type);
    msg->client_action = htonl(msg->client_action);
    msg->server_action = htonl(msg->server_action);
    msg->client_hp = htonl(msg->client_hp);
    msg->server_hp = htonl(msg->server_hp);
    msg->client_torpedoes = htonl(msg->client_torpedoes);
    msg->client_shields = htonl(msg->client_shields);
}

void network_to_host_long(BattleMessage *msg){
    msg->type = ntohl(msg->type);
    msg->client_action = ntohl(msg->client_action);
    msg->server_action = ntohl(msg->server_action);
    msg->client_hp = ntohl(msg->client_hp);
    msg->server_hp = ntohl(msg->server_hp);
    msg->client_torpedoes = ntohl(msg->client_torpedoes);
    msg->client_shields = ntohl(msg->client_shields);
}

void receive_message(int sock_fd, BattleMessage* msg) {
    int numbytes = recv(sock_fd, msg, sizeof(BattleMessage), 0);
    if (numbytes < 0) {
        perror("recv");
        exit(-1);
    }
    network_to_host_long(msg);
}

void send_message(int sock_fd, BattleMessage* msg){
    host_to_network_long(msg);
    int numbytes = send(sock_fd, msg, sizeof(BattleMessage), 0);
    if (numbytes < 0) {
        perror("send");
        exit(1);
    }
    network_to_host_long(msg); 
}