#ifndef client
#define client
#include <string>
#include <mutex>

#include "Handler.h"

class Client : public Handler {
private:
    std::mutex read_mutex;

public:
	int fd;		//TODO: nie bylo - moze zostac?
	int numerPytania;
	int odpowiedz;

	Client(int clientFd, int epoll);
	~Client() override;
	
	void handleEvent(uint32_t events) override;
	
	void sendMessage(std::string message);
};
#endif
