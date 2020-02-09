#include <algorithm> // vector
#inclue <unistd.h>
#include "Server.h"

Server::Server(long port) {
	
	// ======CLEAR=VECTOR======
	clientsVector.clear();
	gameStart = 0;
	
	
	// ======CREATE=SOCKET======
	myAddr = {};
		myAddr.sin_family = AF_INET;
		myAddr.sin_addr.s_addr = hton(INADDR_ANY);
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
	res = listen(server->fd, SOMAXCONN); 
    if(res){
        perror("listen failed");
        return 1;
    }
}

Server::~Server() {
	clientsVector.clear;
	close(fd);
	delete this;
}

void Server::handleEvent(uint32_t events) {
	if (events & EPOLLIN) {
		
		int clientFd = accept(fd, nullptr, nullptr);
		printf("Accepted new connection: %d\n", clientFd);
		
		// lock Mutex
		
		// Dodawanie nowego uczestnika
		Client *client = new Client(clientFd);
		clientsVector.append(clientFd);
		
		printf("Player in game!\n");

		// Jeżeli są wystarczające wymogi do wystartowania gry -> startuje
		if (clientsVector.size() > 0 && gameStart == 0) {
			//mutex unlock;
			// ======NEW=GAME======
			gameStart = 1;
			printf("======STARTING=NEW=GAME======\n");
			std::thread game = std::thread(runGame, this);
			game.detach();
		};
		// else {
		//	mutex unlock
		//};
		 
		 
	}
}

void Server::deleteClient(int clientFd) {
	clientsVector.erase(std::remove(clientsVector.begin(), clientsVector.end(), clientFd), clientsVector.end());
}

void Server::sendToAll(string line) {
	// mutex lock
	for (int it : clientsVector) {
		int count = write(it, &line, strlen(line));
		printf("message sent");
		if(count != (int) strlen(line))
			perror("write failed");
	}

};


void Server::runGame() {
	
	ifstream ifs;
	ifs.open("script.txt", ios::in);
	string line;
	getline(ifs, line);
	
	while (clientsVector.size() > 0) {
		
		int count = write(clientFd, "&&0&&A to jest randomowy tekst\n", strlen("&&0&&A to jest randomowy tekst\n"));
		printf("polaczono z klientem\n");
		if(count != (int) strlen("&&0&&A to jest randomowy tekst\n"))
			perror("write failed");

		count = write(clientFd, "&&1&&0&&A to jest kolejny randomowy tekst\n", strlen("&&1&&2&&A to jest kolejny randomowy tekst\n"));
		printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest kolejny randomowy tekst\n"))
			perror("write failed");

		count = write(clientFd, "&&1&&1&&A to jest 1 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
		printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
			perror("write failed");

		count = write(clientFd, "&&1&&2&&A to jest 2 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
		printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
			perror("write failed");
    
		count = write(clientFd, "&&1&&3&&A to jest 3 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
		printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
			perror("write failed");
    
		count = write(clientFd, "&&1&&4&&A to jest 4 randomowy tekst\n", strlen("&&1&&2&&A to jest 4 randomowy tekst\n"));
		printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 4 randomowy tekst\n"))
			perror("write failed");

		count = write(clientFd, "&&1&&5&&A to jest 5 randomowy tekst\n", strlen("&&1&&2&&A to jest 5 randomowy tekst\n"));
		printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 5 randomowy tekst\n"))
			perror("write failed");
		
		sleep(10);
	}
	gameStart = 0;
}






















