#ifndef handler
#define handler

#include <cstdint>
#include <netinet/in.h>

class Handler {
public:
    int fd;
    int epollFd;

    sockaddr_in myAddr;

    virtual void handleEvent (uint32_t events) = 0;
};
#endif
