#ifndef client
#define client
#include <string>
#include "Handler.h"

class Client : public Handler {
public:
	int fd;		//TODO: nie bylo - moze zostac?

	Client(int clientFd);
	~Client();
	
	void handleEvent(uint32_t events) override;
	
	void sendMessage(std::string message);
};
#endif
