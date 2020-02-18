#include <algorithm> // vector
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <vector>
#include <ctime>
#include <thread>

#include "Client.h"
#include "Server.h"
#include "global.h"

using namespace std;

extern std::string firstAnswer;
extern std::mutex mutex_firstAnswer;

Server::Server(long port, char* host) {

    questionNumber = 0;
    odpowiedz = "";

    mutex_firstAnswer.lock();
    firstAnswer = "";
    mutex_firstAnswer.unlock();
	
	// ======CLEAR=VECTOR======
	clientsVector.clear();
	gameStart = 0;


	// ======CREATE=SOCKET======
	myAddr = {};
		myAddr.sin_family = AF_INET;
		myAddr.sin_addr = {inet_addr(host)};	//TODO: czy tu nie powinno być inet_addr?
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
	res = listen(fd, SOMAXCONN);
    if(res){
        perror("listen failed");
	    exit(0);
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
		
		mutex_vector.lock();
		
		// Dodawanie nowego uczestnika
		Client *clientt = new Client(clientFd, this->getEpoll());
		clientsVector.push_back(clientt);
		
		printf("Player in game!\n");

		// Jeżeli są wystarczające wymogi do wystartowania gry -> startuje
		if (clientsVector.size() > 0 && gameStart == 0) {
			// ======NEW=GAME======
			mutex_vector.unlock();
			sleep(20);
			gameStart = 1;
			printf("======STARTING=NEW=GAME======\n");
			std::thread game (&Server::runGame, this);	//TODO: czy to poprawne?
			game.detach();
		}
		else {
			mutex_vector.unlock();
		};
		 
	}
}

void Server::deleteClient(int clientFd) {
    mutex_vector.lock();
    int i;
    for (i = 0; i < (int) clientsVector.size(); i++) {
        if (clientsVector[i]-> fd == clientFd) {
            break;
        }
    }
    Client *client1 = clientsVector[i];
    delete client1;
    clientsVector.erase(clientsVector.begin() + i);

    mutex_vector.unlock();
}

void Server::sendToAll(char *line) {
	mutex_vector.lock();
	vector <int> invalidClients;
	for (auto &it : clientsVector) {
	    try {
        	int count = write(it->fd, line, strlen(line));
        	printf("message sent %s\n", line);

        	if(count != (int) strlen(line)) {
        		perror("write failed");
        		invalidClients.push_back(it->fd);
        		}

        }catch (...) {
            printf("Error with sendToAll\n");
        }
	}
	/*auto it = clientsVector.begin();
      	while(it != clientsVector.end()) {
      	    try {
              	int count = write(it->fd, line, strlen(line));
              	printf("message sent %s\n", line);

              	if(count != (int) strlen(line)) {
              		perror("write failed");
              		Client * cl = *it;
              		delete cl;
              		it = clientsVector.erase(it);
              	} else
                      it++;
              }catch (...) {
                  printf("Error with sendToAll\n");
              }
      	}*/
	mutex_vector.unlock();
	for (int i = 0; i < (int) invalidClients.size(); i++)
        deleteClient(invalidClients[i]);
    invalidClients.clear();
}


void Server::runGame() {
	
	ifstream ifs;
	ifs.open("script.txt", ios::in);
	string line;
	
	if (clientsVector.size() > 0) {

        int number = 1; string symbol = "Q";
        while (clientsVector.size() > 0 && getline(ifs, line)) {

            if (line != "") {           //empty line is useless for us
                bool correct = true;

                string symbol1 = chooseCode(line, symbol);
                //printf("%s: %s\n", symbol1.c_str(), line.c_str());

                if (symbol1 == "odp") {
                    if (odpowiedz == "") {
                        sleep(10);
                        sendStatistics(0,0,0,0);     //only one sending statistics
                    }

                    if (line.find(odpowiedz) != string::npos) {
                        symbol = "Q";
                        correct = false;
                        }
                    else {
                        correct = false;    //jesli nie znalazło linii - nie ta odpowiedz
                        symbol = symbol1;
                    }
                }
                else if (symbol1 != symbol) {
                    symbol = symbol1;
                    correct = false;
                    }

                if (symbol == "Q" && symbol1 != "odp") {
                    try {
                        odpowiedz = "";
                        number = stoi(&line[0]);
                        questionNumber = number;

                        mutex_firstAnswer.lock();
                        firstAnswer = "";
                        mutex_firstAnswer.unlock();
                        }
                    catch (...) {
                         //printf("In this line there isn't any number\n");
                    }
                }

                if (correct) {
                    string codeLine = codeMessage(line, symbol, number);
                    char mes[codeLine.size() + 1];
                    strcpy(mes, codeLine.c_str());

                    if (correct)
                        sendToAll(mes);
                    if (line.find("Koniec") != string::npos)
                        break;		//zeby nie wysylało pustych lini tylko zakoczyło gre
                        //TODO: wrocic do urochomienia gry i czekac na graczy - po skończeniu serwer mial byc gotowy do kolejnej gry
                }
            }
        }
	}

    mutex_vector.lock();
	auto it = clientsVector.begin();
	while(it != clientsVector.end()) {
	    Client *cl = *it;
	    delete cl;
	    it++;
	}
	clientsVector.clear();
	printf("End Game\n");
	mutex_vector.unlock();

	gameStart = 0;
}

string Server::codeMessage(string line, string symbol, int number) {
	string codeLine = "&&";
    codeLine += string (to_string(number)) + string ("&&") + string(symbol) + string("&&") + string (line);
    return codeLine;
}


string Server::chooseCode(string line, string old) {
    if (line.find(".a.") != string::npos || line.find(".b.") != string::npos || line.find(".c.") != string::npos || line.find(".d.") != string::npos)
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

void Server::chooseMax(int s1, int s2, int s3, int s4) {
     int maxi = max(max(max(s1,s2),s3),s4);
     //printf("%d\n", maxi);
     if ((s1 == maxi && s1 == s2) || (s1 == maxi && s1 == s3) || (s1 == maxi && s1 == s4) || (s3 == maxi && s3 == s2) || (s4 == maxi && s4 == s2) || (s3 == maxi && s3 == s4)) {        //REMIS
        mutex_firstAnswer.lock();
        switch (stoi(firstAnswer)) {
            case 1:
                odpowiedz = ".a.";
                break;
            case 2:
                odpowiedz = ".b.";
                break;
            case 3:
                odpowiedz = ".c.";
                break;
            case 4:
                odpowiedz = ".d.";
                break;
        }
        mutex_firstAnswer.unlock();
     }
     else if (s1 == maxi)
        odpowiedz = ".a.";
     else if (s2 == maxi)
        odpowiedz = ".b.";
     else if (s3 == maxi)
        odpowiedz = ".c.";
     else
        odpowiedz = ".d.";
}

void Server::sendStatistics(int s1, int s2, int s3, int s4) {
    //printf("Statystyki poczatek %d, %d, %d, %d\n", s1, s2, s3, s4);
    for (int i = 0; i < (int) clientsVector.size(); i++ ) {
        if (clientsVector[i]->numerPytania == questionNumber)
            switch (clientsVector[i]->odpowiedz) {
            case 1:
                s1 ++;
                break;
            case 2:
                s2 ++;
                break;
            case 3:
                s3 ++;
                break;
            case 4:
                s4 ++;
                break;
            }
        //printf("%d, %d\n", clientsVector[i]->numerPytania, questionNumber);
    }

    string message = "&&" + to_string(questionNumber) + "&&s1&&" + to_string(s1) + "&&" + to_string(questionNumber) + "&&s2&&" + to_string(s2) + "&&" + to_string(questionNumber) + "&&s3&&" + to_string(s3) + "&&" + to_string(questionNumber) + "&&s4&&" + to_string(s4);
    //printf("Statystyki wynik %d, %d, %d, %d\n", s1, s2, s3, s4);
    if (s1 != 0 || s2 != 0 || s3 != 0 || s4 != 0) {
        chooseMax(s1, s2, s3, s4);
    }
    else odpowiedz = ".a.";     //nie wybrano zadnej odpowiedzi

    char mes[message.size() + 1];
    strcpy(mes, message.c_str());

    sendToAll(mes);
};


















