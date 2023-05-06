#include <sys/socket.h>
#include "context.hpp"
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <glog/logging.h>
#include <vector>

int createListener(const SocketContext &context) {
    int server = socket(context.Domain, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(context.Port);
    serverAddr.sin_family = context.Domain;

    if (bind(server, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        LOG(FATAL) << "bind failed in build server";
    }
    if (listen(server, context.MaxConnections) == -1) {
        close(server);
        LOG(FATAL) << "listen failed in build server";
    }
    return server;
}

int createConnection(const SocketContext &context) {
    int client = socket(context.Domain, SOCK_STREAM, 0);
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(context.Port);
    clientAddr.sin_family = context.Domain;

    if (connect(client, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
        LOG(FATAL) << "connect failed in build client";
    }
    return client;
}

class SocketUtil {
    static int createListener(const SocketContext &context);

    static int createConnect(const SocketContext &context);

    static int accpetConnect(int server);
};