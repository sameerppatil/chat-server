#pragma once

#include <string>
#include "interface.hpp"

using namespace std;

class Client {

    string server_addr;
    uint16_t server_port;
    string user_name;
    Interface client_interface;
public:
    Client(uint16_t _srv_port,
        string _client_name);
    int connectToServer(string _srv_addr);
    int sendData(string _data);
    int readData(char *buf);
    
};
