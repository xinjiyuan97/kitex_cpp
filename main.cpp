#include <iostream>
#include <glog/logging.h>
#include "transport.hpp"
#include "socket.hpp"
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#define MAX_EVENTS 10
using namespace std;

int main(int argc, char **argv) {
    google::SetStderrLogging(google::INFO);
    google::InitGoogleLogging(argv[0]);

    
    
    EpollTransport *transport = new EpollTransport(-1);
    LOG(INFO) << "Server started";

    SocketContext context = {
        .Domain = AF_INET,
        .Host = NULL,
        .Port = 8888,
        .MaxConnections = 5
    };
    int server = createListener(context);
    int epoll_fd;
    if ((epoll_fd = epoll_create1(0)) == -1) {
        close(server);
        LOG(FATAL) << "epoll create1 failed in main";
    }

    struct epoll_event event, events[MAX_EVENTS];
    event.data.fd = server;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server, &event) == -1) {
        close(server);
        LOG(FATAL) << "epoll_ctl failed in main";
    }

    LOG(INFO) << "Server listening on port " << context.Port;
    struct sockaddr_in client_addr;
    int client;
    int n;
    while (true) {
        if ((n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1)) == -1) {
            LOG(ERROR) << "epoll_wait failed in main " << strerror(errno);
            continue;
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == server) {
                socklen_t clientAddrLen = sizeof(client_addr);
                if ((client = accept(server, (struct sockaddr *)&client_addr, &clientAddrLen)) == -1) {
                    LOG(ERROR) << "accept failed in main " << strerror(errno);
                    continue;
                }

                LOG(INFO) << "New client connected from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port);

                // 将新的 client_fd 添加到 epoll 实例中
                event.data.fd = client;
                event.events = EPOLLIN;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1) {
                    LOG(ERROR) << "epoll_ctl failed in main " << strerror(errno);
                    continue;
                }
            } else {
                // 处理 client 的请求
                int client_fd = events[i].data.fd;
                char buf[1024];
                int n;
                if ((n = recv(client_fd, buf, sizeof(buf), 0)) == -1) {
                    LOG(ERROR) << "recv failed in main " << strerror(errno);
                    continue;
                }
                if (n == 0) {
                    LOG(INFO) << "Client disconnected";
                    close(client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    continue;
                }
                LOG(INFO) << "Received " << n << " bytes from client";
                LOG(INFO) << "Message: " << buf;
                if (send(client_fd, buf, n, 0) == -1) {
                    LOG(ERROR) << "send failed in main " << strerror(errno);
                    close(client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    continue;
                }
            }
        }
    }
    close(server);

    // release transport
    delete transport;
    // cout << "Hello World!" << endl;
    return 0;
}