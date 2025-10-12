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

#include "battle_message.h"

#define BACKLOG 1 // the number of connections allowed on the incoming queue

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int status, sockfd, numbytes, client_action, msg_type, round;
    struct addrinfo hints, *res, *p; // will point to the results
    char msg[MSG_SIZE], *ip_addr, *port;
    uint16_t net_action, net_msg_type;

    if (argc != 3) {
        fprintf(stderr, "how to use it: %s <IP address> <port>\n", argv[0]);
        exit(1);
    }

    ip_addr = argv[1];
    port = argv[2];
    
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6 (AF_INET(v4) , AF_INET6(v6)) -> get it from command
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets (fixed)
    
    // servinfo now points to a linked list of 1 or more struct addrinfos (in this case it will point just to one)
    if ((status = getaddrinfo(ip_addr, port, &hints, &res)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
        
    }
    
    for(p = res; p != NULL; p = p->ai_next){
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
			continue;
		}
        
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}
        break;
    }

    freeaddrinfo(res); // free the linked-list

    if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
        exit(1);
	}

    round = 0;
    msg_type = 0;

    do {
        if(round < 2){
            // receive MSG_INIT and MSG_ACTION_RES
            numbytes = recv(sockfd, msg, MSG_SIZE, 0);
            if (numbytes < 0) {
                perror("recv");
                break;
            }
            msg[MSG_SIZE - 1] = '\0';
            printf("%s\n", msg);
        } else {
            printf("%s\n", "Digite sua ação:");
            int result = scanf("%d", &client_action);
            
            // validate action input
            if (result == 0) {
                printf("Ação inválida. Escolha entre 0 e 4.\n");
                continue;
            }
            if ((client_action < 0) | (client_action > 4)) {
                printf("Ação inválida. Escolha entre 0 e 4.\n");
                continue;
            }
            
            net_action = htons(client_action);

            // send the action to server
            numbytes = send(sockfd, &net_action, sizeof(net_action), 0);
            if (numbytes < 0) {
                printf("Falhou ao enviar a mensagem.\n");
                break;
            }

            // receive the type of the message
            numbytes = recv(sockfd, &net_msg_type, sizeof(net_msg_type), 0);
            if (numbytes < 0) {
                perror("recv");
                break;
            }
            msg_type = ntohs(net_msg_type);
    
            // receive the msg itself
            numbytes = recv(sockfd, msg, MSG_SIZE, 0);
            if (numbytes < 0) {
                perror("recv");
                break;
            }
            msg[MSG_SIZE - 1] = '\0';
            printf("%s\n", msg);
        }

        round++;

    } while(msg_type < 5); // different from MSG_GAME_OVER and MSG_ESCAPE


    // receive the msg itself (game over)
    numbytes = recv(sockfd, msg, MSG_SIZE, 0);
    if (numbytes < 0) {
        perror("recv");
    }
    msg[MSG_SIZE - 1] = '\0';
    printf("%s\n", msg);

    // receive the msg itself (inventory)
    numbytes = recv(sockfd, msg, MSG_SIZE, 0);
    if (numbytes < 0) {
        perror("recv");
    }
    msg[MSG_SIZE - 1] = '\0';
    printf("%s\n", msg);


    close(sockfd);
    return 0;
}