#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <time.h>

#include "battle_message.h"
#include "actions_combination.h"

#define BACKLOG 1 // the number of connections allowed on the incoming queue

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    // listen on sock_fd, new connection on new_fd
    int status, sockfd, new_fd, numbytes, server_action;
    struct addrinfo hints, *res, *p; // will point to the results
    struct sockaddr_storage their_addr; // connector's address info
    char s[INET6_ADDRSTRLEN], end_of_round_msg[MSG_SIZE], *protocol, *port;
    uint16_t client_action, net_type;
    socklen_t addr_size;
    
    if (argc != 3) {
        fprintf(stderr, "how to use it: %s <v4|v6> <port>\n", argv[0]);
        exit(1);
    }

    protocol = argv[1];
    port = argv[2];

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets (fixed)
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    if(strcmp(protocol, "v4") == 0){
        hints.ai_family = AF_INET;
    } else if(strcmp(protocol, "v6") == 0){
        hints.ai_family = AF_INET6;
    } else {
        hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
    }

    // res now points to a linked list of 1 or more struct addrinfos (in this case it will point just to one)
    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

    for(p = res; p != NULL; p = p->ai_next){
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
        break;
    }

    freeaddrinfo(res); // free the linked-list

    if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

    if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
    
    while (1) {
        printf("server: waiting for connections...\n");
        
        addr_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_fd == -1) {
            perror("accept");
            exit(1);
        }
        
        // converts clients IP addr to string    
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        BattleMessage battlemsg = {0, 0, 0, 100, 100, 0, 0, ""};

        int round = 0;
        int game_over = 0;

        do {
            if(round < 2){
                // send MSG_INIT and MSG_ACTION_RES
                get_message(&battlemsg);
                numbytes = send(new_fd, battlemsg.message, MSG_SIZE, 0);
                if (numbytes < 0) {
                    perror("send");
                    exit(1);
                }
                battlemsg.type = 1;
            } else {
                // receive client action
                numbytes = recv(new_fd, &client_action, sizeof(client_action), 0);
                if (numbytes < 0) {
                    perror("recv");
                    break;
                }
                
                // generate random action
                server_action = rand() % 5;
                
                battlemsg.server_action = server_action;
                battlemsg.client_action = ntohs(client_action);
                
                // calculate damage and torpedoes and shields
                Combination result = combinations[battlemsg.client_action][battlemsg.server_action];
                battlemsg.client_hp += result.client_damage;
                battlemsg.server_hp += result.server_damage;
                battlemsg.client_torpedoes += (battlemsg.client_action == 1);
                battlemsg.client_shields += (battlemsg.client_action == 2);
                battlemsg.type = 2;

                if((battlemsg.server_action == 4) | (battlemsg.client_action == 4)){
                    battlemsg.type = 6; // set MSG_ESCAPE
                    get_message(&battlemsg);
                } else {
                    // build the message to sent at the end of the turn
                    get_message(&battlemsg); // get MSG_ACTION_RES msg
                    strcpy(end_of_round_msg, battlemsg.message);

                    battlemsg.type = 3;
                    get_message(&battlemsg); // get MSG_BATTLE_RESULT msg

                    // concat the 2 messages
                    strcat(end_of_round_msg, battlemsg.message);

                    strcpy(battlemsg.message, end_of_round_msg);

                    if((battlemsg.client_hp <= 0) | (battlemsg.server_hp <= 0)){
                        game_over = 1;
                        battlemsg.type = 5;
                    }
                }

                // send the msg type
                net_type = htons(battlemsg.type);
                numbytes = send(new_fd, &net_type, sizeof(net_type), 0);
                if (numbytes < 0) {
                    perror("send");
                    exit(1);
                }
    
                // send the msg itself
                numbytes = send(new_fd, battlemsg.message, MSG_SIZE, 0);
                if (numbytes < 0) {
                    perror("send");
                    exit(1);
                }
            }

            round++;

        } while(battlemsg.type < 6 && !game_over);

        battlemsg.type = 5; // set the MSG_GAME_OVER
        get_message(&battlemsg);

        numbytes = send(new_fd, battlemsg.message, MSG_SIZE, 0);
        if (numbytes < 0) {
            perror("send");
            exit(1);
        }

        battlemsg.type = 4; // set the MSG_INVENTORY
        get_message(&battlemsg);

        numbytes = send(new_fd, battlemsg.message, MSG_SIZE, 0);
        if (numbytes < 0) {
            perror("send");
            exit(1);
        }

        close(new_fd);
        break;
    }
    
    close(sockfd);
    return 0;
}
