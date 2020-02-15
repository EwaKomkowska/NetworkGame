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
	
	void codeMessage(int choose);
	
	void sendStatistics(int s1, int s2, int s3, int s4);
};
#endif
