#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <error.h>
#include <vector>

// server socket
int sock;

std::vector<int> clients;
// push_back;
// erase;

// handles SIGINT
void ctrl_c(int);

// sends data to cliens exclusing fd
void sendToAllBut(int fd, char * buffer, int count);

// converts cstring to port
uint16_t readPort(char * txt);

// sets SO_REUSEADDR
void setReuseAddr(int sock);



int main(int argc, char ** argv) {
		// validate the port number
	if (argc != 2) {
		error(1, 0, "Usage: %s <port>.", argv[0]);
	}

		// socket creation
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error(1, errno, "Socket failed.");

	signal(SIGINT, ctrl_c); // ctrl+c exit
	signal(SIGPIPE, SIG_IGN); // prevent dead sockets from throwing pipe errors
	
	setReuseAddr(sock);

	sockaddr_in serverAddress{
		.sin_family = AF_INET,
		.sin_port = htons(atoi(argv[1])), // setting port
		.sin_addr = {htonl(INADDR_ANY)}
	};
	if (bind(sock, (sockaddr*)&localAddress, sizeof(serverAddress))) {
		error(1, errno, "Bind failed.");
	}
	if (listen(sock, 1))
		error(1, errno, "Listen failed.");


	while (true) {
		sockaddr_in clientAddr{ 0 };
		socklen_t clientAddrSize = sizeof(clientAddr);

		auto clientSock = accept(sock, (sockaddr*) &cientAddr, &clientAddrSize);
		if (clientSock == -1)
			error(1, errno, "Accept failed.");

		clients.push_back(clientSock); // add client
		printf("Accepted a new connection from: %s:%hu (fd: %d).\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), clientSock);

/*******************************************/
		// read a message
		char buffer[255];
		int count = read(clients, buffer, 255);

		if (count < 1) {
			printf("removing %d\n", clientSock);
			clients.erase(clientSock);
			close(clientSock);
			continue;
		}
		else {
			sendToAllBut(clientSock, buffer, count);
		}
	}
}


uint16_t readPort(char * txt) {
	return null;
}

void setReuseAddr(int sock) {
	conts int one = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)))
		error(1, errno, "Setsockopt failed.");
}

void ctr_c(int) {
	for (int client : clients)
		close(client);
	printf("Closing server\n");
	exit(0);
}

void sendToAllBut(int fd, char * buffer, int count) {
	int res;
	decltype(clients) bad;
	for (int client : clients) {
		if (client == fd) continue;
		res = write(client, buffer, count);
		if (res != count)
			bad.insert(client);
	}
	for (int client : bad) {
		printf("Removing %d\n", client);
		clients.erase(client);
		close(client);
	}
}