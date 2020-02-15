#include <algorithm> // vector
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
#include <ctime>
#include <thread>
#include <boost/lexical_cast.hpp>       //string -> int

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
	clientsVector.clear();
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
		clientsVector.push_back(clientFd);
		
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
	    try {
        	int count = write(it, line, strlen(line));
        	printf("message sent %s\n", line);

        	if(count != (int) strlen(line))
        		perror("write failed");
        } catch (...) {     //TODO: jaki to blad?
        	printf("Error occured, disconnecting Client %d\n", it);
        	deleteClient(it);
        }
	}

}


void Server::runGame() {
	
	ifstream ifs;
	ifs.open("script.txt", ios::in);
	string line;
	
	if (clientsVector.size() > 0) {	//TODO: czy tu nie ma byc petli po wszystkich wartosciach vectora? na razie dodaje tylko wysyłanie do 0 elementu - było clientFd

		int count = write(clientsVector[0], "&&1&&A&&A to zmieniony tekst\n", strlen("&&1&&Q&&A to zmieniony tekst\n"));
		if(count != (int) strlen("&&1&&Q&&A to zmieniony tekst\n"))
			perror("write failed");
		sleep(1);

        //wysłanie statystyk 		//TODO: ogarnac to!!!
		count = write(clientsVector[0], "&&1&&s4&&Stat4\n&&1&&s3&&Stat3\n&&1&&s2&&Stat2\n&&1&&s1&&Stat1\n", strlen("&&1&&Qs&&Stat2\n&&1&&s3&&Stat3\n&&1&&s2&&Stat2\n&&1&&s1&&Stat1\n"));
		if(count != (int) strlen("&&1&&Qs&&Stat2\n&&1&&s3&&Stat3\n&&1&&s3&&Stat3\n&&1&&s3&&Stat3\n"))
			perror("write failed");
		sleep(1);

        count = write(clientsVector[0], "&&1&&s3&&Stat3\n", strlen("&&1&&Qs&&Stat2\n"));
		if(count != (int) strlen("&&1&&sQ&&Stat2\n"))
			perror("write failed");
		sleep(1);

        count = write(clientsVector[0], "&&1&&s2&&Stat2\n", strlen("&&1&&sQ&&Stat2\n"));
		if(count != (int) strlen("&&1&&Qs&&Stat2\n"))
			perror("write failed");
		sleep(1);

        count = write(clientsVector[0], "&&1&&s1&&Stat1\n", strlen("&&1&&Q1&&Stat2\n"));
		if(count != (int) strlen("&&1&&1Q&&Stat1\n"))
			perror("write failed");
	}

    int number = 1; string symbol = "Q";
	while (clientsVector.size() > 0 && getline(ifs, line)) {	//TODO: wydobyc numer pytania i wysylac statystyki

        if (line != "") {           //empty line is useless for us
            bool correct = true;

            symbol = chooseCode(line, symbol);

            if (symbol == "Q") {
                try {
                number = stoi(&line[0]);
                printf("%i\n", number);
                } catch (...) {
                    //printf("In this line there isn't any number\n");
                }
            } else if (symbol == "odp") {
                //TODO: dodac warunek  && line.find(poprawne_glosowanie)
                symbol = "Q";
            }

            string codeLine = codeMessage(line, symbol, number);
            char mes[codeLine.size() + 1];
            strcpy(mes, codeLine.c_str());

            //if (correct)      //TODO: jak sprawdzic czy to jest linia z numerem czy tekstem
            sendToAll(mes);

            sleep(1);

            if (line.find("Koniec") != string::npos)
                break;		//zeby nie wysylało pustych lini tylko zakoczyło gre
                //TODO: wrocic do urochomienia gry i czekac na graczy - po skończeniu serwer mial byc gotowy do kolejnej gry
        }
	}
	gameStart = 0;
}

string Server::codeMessage(string line, string symbol, int number) {
	string codeLine = "&&";
    codeLine += string (to_string(number)) + string ("&&") + string(symbol) + string("&&") + string (line);
    return codeLine;
}


string Server::chooseCode(string line, string old) {
    if (line.find(".a.") != string::npos || line.find(".b.") != string::npos || line.find(".c.") != string::npos || line.find(".d.") != string::npos)
        //TODO: wysylamy tylko jedną odpowiedź - sprawdzic czy to dobre
        return "odp";

    if (line.find(".Q.") != string::npos || line.find("Koniec") != string::npos)   //tutaj wysylamy - numer pytania sie przyda!!!
        return "Q";

    if (line.find(".A.") != string::npos)
        return "A";

    if (line.find(".B.") != string::npos)
        return "B";

    if (line.find(".C.") != string::npos)
        return "C";

    if (line.find(".D.") != string::npos)
        return "D";

    return old;     //last symbol is good now
}

void Server::sendStatistics(int s1, int s2, int s3, int s4) {
	/*
	&&1&&stat1&&10&&1&&stat2&&20

	pyt.1 = 10%
	pyt.2 = 20%
	*/
};



















