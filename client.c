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

#define BACKLOG 1 // the number of connections allowed on the incoming queue

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    // listen on sock_fd, new connection on new_fd
    int status, sockfd, new_fd;
    struct addrinfo hints, *res, *p; // will point to the results
    struct sockaddr_storage their_addr; // connector's address info
    socklen_t addr_size;
    char s[INET6_ADDRSTRLEN], *ip_addr, *port;

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
        
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
        printf("client: attempting connection to %s\n", s);

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}
        break;
    }

    if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
        exit(1);
	}

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	printf("client: connected to %s\n", s);

    freeaddrinfo(res); // free the linked-list

    close(sockfd);

    return 0;
}