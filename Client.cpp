#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>

#include "Client.h"
#include "Server.h"

using namespace std;

Client::Client(int clientFd) {
	fd = clientFd;		//TODO: brak deklaracji, ze to int?
	epoll_event event{EPOLLIN | EPOLLRDHUP, {.ptr=this}};
	epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
}

Client::~Client() {
	// delete from vector
	//Server::deleteClient(this->fd);		//TODO: cos nie dziala - nie trzeba przechowywac serwera?
	epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
	shutdown(fd, SHUT_RDWR);
	close(fd);
}

void Client::handleEvent(uint32_t events) {
	if (events & EPOLLIN) {
		// otrzymano wiadomość od klienta
		
		char buffer[50];
		int count = read(fd, buffer, sizeof(buffer));
		// mutex lock
		
		//! AND GAME STARTED
		if (count > 0) {
			//! Tutaj będzie potrzebna obsługa odpowiedzi
			//&&x&&4;
			std::string message = std::string(buffer);
			printf("Otrzymano wiadomość %s", message.c_str());
		}
		
		// mutex unlock
	};
}

void Client::sendMessage(std::string message) {
	write(fd, &message, message.length());
};
