#include <string>

struct SocketContext {
    int Domain; // AF_INET or AF_INET6
    const std::string *Host; //
    int Port;

    const int MaxConnections;
};