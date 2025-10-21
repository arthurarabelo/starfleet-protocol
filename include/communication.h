#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "battle_message.h"

void host_to_network_long(BattleMessage *msg);
void network_to_host_long(BattleMessage *msg);
void receive_message(int sock_fd, BattleMessage* msg);
void send_message(int sock_fd, BattleMessage* msg);

#endif
