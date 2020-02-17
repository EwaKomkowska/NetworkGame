#ifndef handler
#define handler

#include <cstdint>
#include <netinet/in.h>

class Handler {
private:
    int epollFd;

public:
    int fd;
    sockaddr_in myAddr;

    virtual void handleEvent (uint32_t events) = 0;

    int getEpoll() {
        return epollFd;
    }

    void setEpoll(int epoll) {
        epollFd = epoll;
    }
};
#endif
