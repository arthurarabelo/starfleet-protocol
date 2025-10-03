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

int main(int argc, char *argv[]) {
    // listen on sock_fd, new connection on new_fd
    int status, sockfd, new_fd;
    struct addrinfo hints, *res, *p; // will point to the results
    struct sockaddr_storage their_addr; // connector's address info
    socklen_t addr_size;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6 (AF_INET(v4) , AF_INET6(v6)) -> get it from command
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets (fixed)
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me (don't actually need this, get the ip from command line)

    // servinfo now points to a linked list of 1 or more struct addrinfos (in this case it will point just to one)
    if ((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
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

    if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

    if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

    printf("server: waiting for connections...\n");

   while(1) {  // main accept() loop
		addr_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

        // converts clients IP addr to string    
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
		printf("server: got connection from %s\n", s);
    }

    freeaddrinfo(res); // free the linked-list

    return 0;
}