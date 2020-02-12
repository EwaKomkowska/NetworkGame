#include <algorithm> // vector
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
#include <ctime>
#include <thread>

#include "Client.h"
#include "Server.h"

using namespace std;

Server::Server(long port) {
	
	// ======CLEAR=VECTOR======
	clientsVector.clear();
	gameStart = 0;
	
	
	// ======CREATE=SOCKET======
	myAddr = {};
		myAddr.sin_family = AF_INET;
		myAddr.sin_addr.s_addr = htons(INADDR_ANY);	//TODO: czy tu nie powinno być inet_addr?
		myAddr.sin_port = htons((uint16_t)port);
		
	fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket failed");
        exit(0);
    }
 
	// ======BIND=SOCKET======
    int res = bind(fd, (sockaddr*) &myAddr, sizeof(myAddr));
    if(res){
        perror("bind failed");
        exit(0);
    }
	
	// ======LISTEN======
	res = listen(fd, SOMAXCONN); 		//TODO: było server->fd
    if(res){
        perror("listen failed");
        //return 1;
	exit(0);		//TODO: czy tak moze zostac
    }
}

Server::~Server() {
	clientsVector.clear();	//TODO: nie lepiej erase od razu?
	close(fd);
	delete this;
}

void Server::handleEvent(uint32_t events) {
	if (events & EPOLLIN) {
		
		int clientFd = accept(fd, nullptr, nullptr);
		printf("Accepted new connection: %d\n", clientFd);
		
		// lock Mutex
		
		// Dodawanie nowego uczestnika
		Client *clientt = new Client(clientFd);
		clientsVector.push_back(clientFd);		//TODO: bylo append - moze byc tak?
		
		printf("Player in game!\n");

		// Jeżeli są wystarczające wymogi do wystartowania gry -> startuje
		if (clientsVector.size() > 0 && gameStart == 0) {
			//mutex unlock;
			// ======NEW=GAME======
			gameStart = 1;
			printf("======STARTING=NEW=GAME======\n");
			std::thread game (&Server::runGame, this);	//TODO: czy to poprawne?
			game.detach();
		};
		// else {
		//	mutex unlock
		//};
		 
		 
	}
}

void Server::deleteClient(int clientFd) {
	clientsVector.erase(std::remove(clientsVector.begin(), clientsVector.end(), clientFd), 		clientsVector.end());
}

void Server::sendToAll(string line) {
	// mutex lock
	for (int it : clientsVector) {
		int count = write(it, &line, line.length());
		printf("message sent");
		if(count != (int) line.length())
			perror("write failed");
	}

}


void Server::runGame() {
	
	ifstream ifs;
	ifs.open("script.txt", ios::in);
	string line;
	getline(ifs, line);
	
	while (clientsVector.size() > 0) {	//TODO: czy tu nie ma byc petli po wszystkich wartosciach vectora? na razie dodaje tylko wysyłanie do 0 elementu - było clientFd
		
		int count = write(clientsVector[0], "&&0&&A to jest randomowy tekst\n", strlen("&&0&&A to jest randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&0&&A to jest randomowy tekst\n"))
			perror("write failed");

		count = write(clientsVector[0], "&&1&&A&&A to jest kolejny randomowy tekst\n", strlen("&&1&&2&&A to jest kolejny randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&A&&A to jest kolejny randomowy tekst\n"))
			perror("write failed");

		count = write(clientsVector[0], "&&1&&B&&A to jest 1 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&B&&A to jest 4 randomowy tekst\n"))
			perror("write failed");

		count = write(clientsVector[0], "&&1&&C&&A to jest 2 randomowy tekst\n", strlen("&&1&&C&&A to jest 4 randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
			perror("write failed");
    
		count = write(clientsVector[0], "&&1&&D&&A to jest 3 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
			perror("write failed");
    
		count = write(clientsVector[0], "&&1&&Q&&A to jest 4 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
			perror("write failed");

		count = write(clientsVector[0], "&&1&&5&&A to jest 5 randomowy tekst\n", strlen("&&1&&2&&A to jest 5 randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 5 randomowy tekst\n"))
			perror("write failed");

		printf("Wyslano wszystkie dane do klienta\n");
		
		sleep(10);
	}
	gameStart = 0;
}




















