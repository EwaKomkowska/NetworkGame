#ifndef server
#define server
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string>
#include <vector>

#include "Handler.h"

class Server : public Handler {
public:
	std::vector<int> clientsVector;
	int gameStart;
	
	Server(long port);
	~Server();
	
	void handleEvent(uint32_t events) override;
	
	void deleteClient(int clientFd);
	
	void sendToAll(char *line);

	void runGame();

	std::string codeMessage(std::string line, std::string symbol, int number);

	std::string chooseCode(std::string line, std::string old);

    void sendStatistics(int s1, int s2, int s3, int s4);
};
#endif
