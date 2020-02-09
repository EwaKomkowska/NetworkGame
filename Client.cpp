#include "Server.h"

Client::Client(int clientFd) {
	fd = clientFd;
	epoll_event event{EPOLLIN | EPOLLRDHUP, {.ptr=this}};
	epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
}

Client::~Client() {
	// delete from vector
	Server::deleteClient(fd);
	epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
	shutdown(fd, SHUT_RDWR);
	close(fd);
}

void Client::handleEvent(uint32_t events) {
	if (events & EPOLLIN) {
		// otrzymano wiadomość od klienta
		
		char buffer[50];
		int count = readData(fd, buffer, sizeof(buffer));
		// mutex lock
		
		//! AND GAME STARTED
		if (count > 0) {
			//! Tutaj będzie potrzebna obsługa odpowiedzi
			//&&x&&4;
			std::string message = std::string(buffer);
			printf("Otrzymano wiadomość %s", message);
		}
		
		// mutex unlock
	};
}

void Client::sendMessage(std::string message) {
	write(fd, &message, strlen(message));
};