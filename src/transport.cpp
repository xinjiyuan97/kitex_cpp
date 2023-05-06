#include "transport.hpp"

EpollTransport::EpollTransport(int MaxConn = 10, int MaxEvents = 10) {
    this->MaxConn = MaxConn;
    this->MaxEvents = MaxEvents;
    this->events = std::vector<struct epoll_event>(MaxEvents);

    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd == -1) {
        LOG(FATAL) << "epoll create1 failed in EpollTransport::EpollTransport";
    }
    
}

void EpollTransport::OnConnect(const struct epoll_event &event) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int client;
    if ((client = accept(this->fd, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1) {
        LOG(ERROR) << "accept failed in main " << strerror(errno);
        return;
    }

    LOG(INFO) << "New client connected from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port);
    struct epoll_event connEvent;
    // 将新的 client_fd 添加到 epoll 实例中
    connEvent.data.fd = client;
    connEvent.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &connEvent) == -1) {
        LOG(ERROR) << "epoll_ctl failed in main " << strerror(errno);
        return;
    }
    return;
}

void EpollTransport::OnRead(const struct epoll_event &event) {
    int clientFd = event.data.fd;
    char buf[1024];
    int n;
    if ((n = recv(clientFd, buf, sizeof(buf), 0)) == -1) {
        LOG(ERROR) << "recv failed in main " << strerror(errno);
        return;
    }
    if (n == 0) {
        LOG(INFO) << "Client disconnected";
        close(clientFd);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientFd, NULL);
        return;
    }
    LOG(INFO) << "Received " << n << " bytes from client";
    if (send(clientFd, buf, n, 0) == -1) {
        LOG(ERROR) << "send failed in main " << strerror(errno);
        close(clientFd);
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientFd, NULL);
        return;
    }
    
    return;
}

void EpollTransport::Start(int fd) {
    this->fd = fd;
    this->event.events = EPOLLIN;
    this->event.data.fd = this->fd;
    epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->fd, &this->event);
    int n;
    while (true) {
        if ((n = epoll_wait(epoll_fd, events, this->MaxEvents, -1)) == -1) {
            LOG(ERROR) << "epoll_wait failed in main " << strerror(errno);
            continue;
        }

        for (int i = 0; i < n; i++) {
        if (events[i].data.fd == this->fd) {
            this->OnConnect(events[i]);
        } else {
           this->OnRead(events[i]);
        }
    }
}
    return ;
}

void EpollTransport::Stop() {
    return ;
}