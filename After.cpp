#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <csignal>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/epoll.h>
#include <vector>

#include "Server.h"
#include "Handler.h"

Server * server1;

using namespace std;

//! Komentarze z takim oznaczeniem są wskazówką w razie błędów kompilacji

int main(int argc, char ** argv) {
    if(argc!=2){
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    char * endp;
    long port = strtol(argv[1], &endp, 10);
    if(*endp || port > 65535 || port < 1){
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    
	server1 = new Server(port);
	
	// =======EPOLL=EVENTS=CATCH========
	
	server1->epollFd = epoll_create1(0);
	epoll_event event{EPOLLIN, {.ptr=server1}};
	epoll_ctl(server1->epollFd, EPOLL_CTL_ADD, server1->fd, &event);
	
	// =========HANDLE=EVENTS===========
	
	while(1) {
		if (-1 == epoll_wait(server1->epollFd, &event, 1, -1)) {
			perror("epoll_wait failed");
			close(server1->fd);
			delete server1;
			exit(0);
		}
		((Handler *)event.data.ptr) -> handleEvent(event.events);
	}
	
	// ======CLOSE======
	close(server1->fd);
	delete server1;
	return 0;
	
}







