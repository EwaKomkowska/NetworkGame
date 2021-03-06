#include <cstdio>
#include <string>
#include <vector>
#include <unistd.h>

#include "Client.h"
#include "Server.h"

extern std::string firstAnswer;
extern std::mutex mutex_firstAnswer;

using namespace std;

Client::Client(int clientFd, int epoll) {
	fd = clientFd;
	epoll_event event{EPOLLIN | EPOLLRDHUP, {.ptr=this}};
	epoll_ctl(epoll, EPOLL_CTL_ADD, fd, &event);
}

Client::~Client() {
	epoll_ctl(getEpoll(), EPOLL_CTL_DEL, fd, nullptr);
	try {
	    shutdown(fd, SHUT_RDWR);
	    close(fd);
	} catch (...) {}
}

void Client::handleEvent(uint32_t events) {
	if (events & EPOLLIN) {
		// otrzymano wiadomość od klienta
		char buffer[100];

		read_mutex.lock();
		int count = read(fd, buffer, sizeof(buffer));

		
		//! AND GAME STARTED
		if (count > 0) {
			//! Obsługa odpowiedzi
			//&&x&&4;
			std::string message = std::string(buffer);
			printf("Otrzymano wiadomość %d: %s\n", count, message.c_str());

			numerPytania = stoi(&message[2]);
			odpowiedz = stoi(&message[5]);
			mutex_firstAnswer.lock();
            if (firstAnswer == "") {
                firstAnswer = &message[5];
                printf("Zmienilem pierwsza wiadomosc\n");
                }
            mutex_firstAnswer.unlock();
		}
		else {
		    numerPytania = 0;
		    odpowiedz = 0;
		}
		
		read_mutex.unlock();
	};
}
