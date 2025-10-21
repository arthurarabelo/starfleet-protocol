#define _POSIX_C_SOURCE 200112L 

#include <stdio.h>
#include <string.h>
#include <netdb.h>

#include "communication.h"

#define BACKLOG 1 // the number of connections allowed on the incoming queue

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int status, sockfd, client_action;
    struct addrinfo hints, *res, *p; // will point to the results
    char *ip_addr, *port;
    BattleMessage msg = {0, 0, 0, 100, 100, 0, 0, "", 0};

    if (argc != 3) {
        fprintf(stderr, "how to use it: %s <IP address> <port>\n", argv[0]);
        exit(1);
    }

    ip_addr = argv[1];
    port = argv[2];
    
    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6 (AF_INET(v4) , AF_INET6(v6))
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

    int loop = 0;

    do {
        if(loop < 2){
            // receive MSG_INIT and MSG_ACTION_RES
            receive_message(sockfd, &msg);
            msg.message[MSG_SIZE - 1] = '\0';
            printf("%s\n", msg.message);
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

            msg.client_action = client_action;
            
            // send the action to server
            send_message(sockfd, &msg);

            // receive the msg itself
            receive_message(sockfd,& msg);
            msg.message[MSG_SIZE - 1] = '\0';
            printf("%s\n", msg.message);
        }

        loop++;

    } while(msg.type < 5); // different from MSG_GAME_OVER and MSG_ESCAPE

    // receive the msg itself (game over)
    receive_message(sockfd, &msg);
    msg.message[MSG_SIZE - 1] = '\0';
    printf("%s\n", msg.message);

    // receive the msg itself (inventory)
    receive_message(sockfd, &msg);
    msg.message[MSG_SIZE - 1] = '\0';
    printf("%s\n", msg.message);

    close(sockfd);
    return 0;
}