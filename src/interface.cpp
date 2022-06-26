
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <cassert>

#include "interface.hpp"
struct mySockAddr {
    int a;
};
Interface::Interface(const string &addr_str, uint16_t port) : socket_fd{-1}, other_end_fd{-1} {
    // if addr is provided, this is likely for serer addr
    // so we need to convert the the str to address
    auto ret = socket(AF_INET, SOCK_STREAM, 0);
    assertm(ret == 0, "unable to create socket for peer connection");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    ret = inet_pton(AF_INET, const_cast<char *>(addr_str.c_str()), &(addr.sin_addr));
    assertm(ret > 0, "failed to convert provided addr string to addr");
}

Interface::Interface(uint16_t port, bool is_socket_passive) : socket_fd{-1}, other_end_fd{-1} {
    this->port = port;
    
    int opt = -1;
    int ret;

    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    assertm(this->socket_fd != 0, "unable to get socket");

    if(is_socket_passive) {
        ret = setsockopt(this->socket_fd,
            SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        assertm(ret == 0, "unable to get socket");
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(this->port);

        ret = bind(this->socket_fd, reinterpret_cast<const sockaddr*>(&addr),
                sizeof(addr));
        // if(ret 1)
        assertm(ret == 0, "unable to bind");
        PLOGD << "Successfully binded PASSIVE inteface to port: " << this->port;
    }
    else {
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_LOOPBACK;
        // inet_pton(AF_INET, "127.0.0.0", &(addr.sin_addr));
        addr.sin_port = htons(8111);
        PLOGD << "Successfully binded ACTIVE inteface to port: " << this->port;
    }
}

void Interface::startListening() {
    interface_passive = true;
    auto ret = listen(socket_fd, kListeningBacklog);
    assertm(ret == 0, "unable to start listening");
}

// void Interface::connect(int _other_end_socket) {
//     /**
//      * @brief connect() and listen() cannot be applied to same socket.
//      * If you call connect(), it tries to reach the peer then and there.
//      * While listen() marks that socket as passive. On such socket, you
//      * cannot call connect.
//      */
    
//     assertm(interface_passive == false, "cannot connect to PASSIVE interface");
//     PLOGD << "calling connect" << endl;
//     other_end_fd = connect(_other_end_socket,
//                            reinterpret_cast<const sockaddr *>(&addr), sizeof(addr));
//     return;
// }

int Interface::connectTo(string& addr_str) {
    assertm(interface_passive == false, "cannot connect to PASSIVE interface");

    provided_addr = {addr_str};
    auto ret = inet_pton(AF_INET, const_cast<char *>(addr_str.c_str()), &addr.sin_addr);
    assertm(ret > 0, "failed to convert provided addr string to addr");
    this->other_end_fd = connect(this->socket_fd, reinterpret_cast<const struct sockaddr *>(&addr),
                                 sizeof(addr));
    /* auto sock = socket(AF_INET, SOCK_STREAM, 0);
    this->other_end_fd = connect(sock, (struct sockaddr *)&addr,
                             sizeof(addr)); */
    if(this->other_end_fd < 0 )
    {
        PLOGE << "failed to connect to server on " << addr_str << ", port: " << this->port <<
                ", returned: " << this->other_end_fd;
        return -1;
    }
    return 0;
}

int Interface::sendDataTo(int _toSock, string _data) {
    // TODO: fix the count bit of write
    auto ret = send(_toSock, _data.c_str(), _data.length(), 0);
    return ret;
}

int Interface::sendData(string _data){
    // TODO: fix the count bit of write
    auto ret = send(this->socket_fd, _data.c_str(), _data.length(), 0);
    return ret;
}

int Interface::acceptConnections(string& peer_ip_addr) {
    struct sockaddr peer_addr;
    socklen_t curr_addr_sz = sizeof(peer_addr);
    PLOGD << "waiting on accept";
    auto ret = accept(socket_fd, &peer_addr, &curr_addr_sz);
    if(ret < 0) {
        PLOGD << "failed to get FD for peer connection";
    }
    else {
        char addr_str[INET_ADDRSTRLEN];
        if(inet_ntop(AF_INET, &peer_addr, addr_str, INET_ADDRSTRLEN) != nullptr) {
            // peer_ip_addr = to_string(addr_str);
            peer_ip_addr = addr_str;
            PLOGD << "got message from " << addr_str;
        }
        else {
            PLOGD << "NULL PTR ";
        }
    }
    PLOGD << "accept unblocked";
    return ret;
}

int Interface::readData(char *buf, int _socket) {
    auto result = read(_socket, buf, 1024);
    return result;
}

int Interface::readData(char *buf) {
    auto result = read(this->socket_fd, buf, 1024);
    return result;
}

Interface::~Interface()
{
    if(socket_fd != -1)
        close(socket_fd);
    PLOGD << "Closing socket";
}