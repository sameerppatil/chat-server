#pragma once
#include <string_view>
#include "interface.hpp"
#include "common.hpp"

using namespace std;

class Server {
private:
    Interface interface;
    static constexpr uint16_t kPort = 8111;
public:
    Server(uint16_t port);
    Server();
    ~Server() {}
    void initializeServer(void);
    void startRunning(void);
};