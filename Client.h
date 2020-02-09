#include <string>
#include "Handler.h"

class Client : public Handler {
public:
	Client(int clientFd);
	~Client() override;
	
	void handleEvent(uint32_t events);
	
	void sendMessage(std::string message);
}