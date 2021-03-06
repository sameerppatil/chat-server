#include <thread>
#include <chrono>
#include "client.hpp"

using namespace chrono_literals;

Client::Client(uint16_t _srv_port,
       string _client_name) :
       server_port{_srv_port}, user_name{_client_name},
       m_connected_to_server{false},
       client_interface(_srv_port, false)  {}

int Client::connectToServer(string server_addr) {
    if(m_connected_to_server) {
        // we are already connected;
        return -1;
    }
    for(int i = 0; i < 50; i++) {
        auto ret = client_interface.connectTo(server_addr);
        if(ret != 0 ) {
            PLOGD << "Server not found " << i << " instance...";
            this_thread::sleep_for(500ms);
        }
        else {
            m_connected_to_server = true;
            break;
        }
    }
    return -1;
}

int Client::sendData(string _data) {
    return client_interface.sendData(_data);
}

int Client::readData(char* buf) {
    return client_interface.readData(buf);
}

bool Client::isConnectedToServer() {
    return m_connected_to_server;
}