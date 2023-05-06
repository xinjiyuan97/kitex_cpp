#include <sys/epoll.h>
#include <stdexcept>
#include <glog/logging.h>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>

class Transport {
public:
    virtual void Start(int fd);
    virtual void Stop();
};

class EpollTransport : public Transport {
public:
    EpollTransport(int MaxConn = 10, int MaxEvents = 10);
    void OnConnect(const struct epoll_event &event);
    void OnRead(const struct epoll_event &event);

    void Start(int fd);
    void Stop();

private:
    int fd;
    int epoll_fd;
    int MaxConn, MaxEvents;
    struct epoll_event event;
    vector<struct epoll_event> events;
};
