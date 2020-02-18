#ifndef client
#define client
#include <string>
#include <mutex>

#include "Handler.h"

class Client : public Handler {
private:
    std::mutex read_mutex;

public:
	int fd;
	int numerPytania;
	int odpowiedz;

	Client(int clientFd, int epoll);
	~Client() override;
	
	void handleEvent(uint32_t events) override;
};
#endif
