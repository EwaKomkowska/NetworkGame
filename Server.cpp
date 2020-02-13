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

void Server::sendToAll(char *line) {
	// mutex lock
	for (int it : clientsVector) {
		int count = write(it, line, strlen(line));
		printf("message sent\n");
		if(count != (int) strlen(line))
			perror("write failed");
	}

}


void Server::runGame() {
	
	ifstream ifs;
	ifs.open("script.txt", ios::in);
	string line;
	getline(ifs, line);
	
	if (clientsVector.size() > 0) {	//TODO: czy tu nie ma byc petli po wszystkich wartosciach vectora? na razie dodaje tylko wysyłanie do 0 elementu - było clientFd
//bylo while, ale teraz chce miec z pliku
		
		/*tutaj testowane randomowe teksty 
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

		count = write(clientsVector[0], "&&1&&Q&&A to jest 5 randomowy tekst\n", strlen("&&1&&2&&A to jest 5 randomowy tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&2&&A to jest 5 randomowy tekst\n"))
			perror("write failed");

		printf("Wyslano wszystkie dane do klienta\n");
		
		sleep(10);*/

		int count = write(clientsVector[0], "&&1&&A&&A to zmieniony tekst\n", strlen("&&1&&Q&&A to zmieniony tekst\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&Q&&A to zmieniony tekst\n"))
			perror("write failed");
		printf("Druga partia też\n");
		sleep(10);

//wysłanie statystyk 		//TODO: ogarnac to!!!
		count = write(clientsVector[0], "&&1&&s4&&Stat4\n", strlen("&&1&&Qs&&Stat2\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&Qs&&Stat2\n"))
			perror("write failed");
		printf("Druga partia też\n");
		sleep(10);

count = write(clientsVector[0], "&&1&&s3&&Stat3\n", strlen("&&1&&Qs&&Stat2\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&sQ&&Stat2\n"))
			perror("write failed");
		printf("Druga partia też\n");
		sleep(10);

count = write(clientsVector[0], "&&1&&s2&&Stat2\n", strlen("&&1&&sQ&&Stat2\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&Qs&&Stat2\n"))
			perror("write failed");
		printf("Druga partia też\n");
		sleep(10);

count = write(clientsVector[0], "&&1&&s1&&Stat1\n", strlen("&&1&&Q1&&Stat2\n"));
		//printf("polaczono z klientem\n");
		if(count != (int) strlen("&&1&&1Q&&Stat1\n"))
			perror("write failed");
		printf("Druga partia też\n");
	}


	printf("Obudziłem sie %s\n", line.c_str());
	int numer=1; string sym="Q";
	while (clientsVector.size() > 0) {
		do {		//TODO: wydobyc numer pytania i znacznik
		printf("Linia %s\n", line.c_str());
		bool poprawne = true;
			if (line.length() > 0) {
				if (line.find(".a.") != string::npos || line.find(".b.") != string::npos || line.find(".c.") != string::npos || line.find(".d.") != string::npos) {
					//TODO: wysylamy tylko jedną odpowiedź - sprawdzic czy to dobre
					sym = "Q";
					line = " ";		//zeby nie wywalilo bledu na pusty, a bylo widoczne, ze to nowa czesc
					poprawne = false;
				}
				else if (line.find(".Q.") != string::npos || line.find("Koniec") != string::npos) //tutaj wysylamy - numer pytania sie przyda!!!
					sym="Q";
				else if (line.find(".A.") != string::npos) {
					sym = "A";
					poprawne = false;
				}
				else if (line.find(".B.") != string::npos) {
					sym = "B";
					poprawne = false;
				}
				else if (line.find(".C.") != string::npos) { 
					sym = "C";
					poprawne = false;
				}
				else if (line.find(".D.") != string::npos) {
					sym = "D";
					poprawne = false;
				}
			string codeLine = "&&";
			codeLine += string (to_string(numer)) + string ("&&") + string(sym) + string("&&") + string (line);
				char mes[codeLine.size() + 1];
				strcpy(mes, codeLine.c_str());
				if (poprawne)
					sendToAll(mes);
				printf("wyslalem %s\n", mes);
				sleep(10);
				if (line.find("Koniec") != string::npos) break;		//zeby nie wysylało pustych lini tylko zakoczyło gre
			}
		} while (getline(ifs, line));		//TODO: czy na pewno skoczy sie na eof?
		break;
	}
	gameStart = 0;
}




















