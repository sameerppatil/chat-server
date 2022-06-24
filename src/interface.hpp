#pragma once

#include <string>
#include <string_view>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "common.hpp"

using namespace std;

class Interface
{
private:
    /* data */
    int socket_fd;
    uint16_t port;
    string provided_addr;
    static constexpr const int kListeningBacklog = 5;
    bool interface_passive = false;
    struct sockaddr_in addr;
    int other_end_fd;
public:
    Interface(const string &addr, uint16_t port);
    Interface(uint16_t port, bool is_socket_passive);
    // void connect(int _socket);
    int connectTo(string& _socket);
    int sendData(string _data);
    void startListening();
    int acceptConnections();
    int readData(char *buf, int _socket);
    int readData(char *buf);
    int sendDataTo(int _socket, string msg);
    ~Interface();
};
