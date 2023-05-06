#include <sys/socket.h>
#include "context.hpp"
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <glog/logging.h>
#include <vector>


class SocketUtil {
public:
    static int createListener(const SocketContext &context);

    static int createConnect(const SocketContext &context);

    static int accpetConnect(int server);
};