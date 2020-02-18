#ifndef server
#define server

#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <mutex>

#include "Handler.h"
#include "Client.h"


class Server : public Handler {
private:
    std::mutex mutex_vector;
    std::mutex mutex_statistics;
	std::string odpowiedz;

public:
	std::vector<Client*> clientsVector;
	int gameStart;
	int questionNumber;
	
	Server(long port, char* host);
	~Server() override;
	
	void handleEvent(uint32_t events) override;
	
	void deleteClient(int clientFd);
	
	void sendToAll(char *line);

	void runGame();

	std::string codeMessage(std::string line, std::string symbol, int number);

	std::string chooseCode(std::string line, std::string old);

    void add(int pytanie, int odp, int ile);

    void sendStatistics(int s1, int s2, int s3, int s4);

    void chooseMax(int s1, int s2, int s3, int s4);
};
#endif
