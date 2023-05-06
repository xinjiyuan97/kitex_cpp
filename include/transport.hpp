#include <sys/epoll.h>
#include <stdexcept>
#include <glog/logging.h>

class Transport {
    public:
        virtual void send() = 0;
        virtual void recv() = 0;
        virtual void close() = 0;
};

class EpollTransport : public Transport {
    public:
        EpollTransport(int fd);
        void send();
        void recv();
        void close();
    private:
        int fd;
        int epoll_fd;
        struct epoll_event event;
};
