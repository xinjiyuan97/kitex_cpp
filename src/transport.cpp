#include "transport.hpp"

EpollTransport::EpollTransport() {
    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd == -1) {
        LOG(ERROR) << "epoll create1 failed in EpollTransport::EpollTransport";
        throw std::runtime_error( "epoll create1 failed in EpollTransport::EpollTransport");
    }
    this->event.events = EPOLLIN;
    this->event.data.fd = this->fd;
    epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->fd, &this->event);
}