#include "server.hpp"

void Server::initializeServer(void)
{

}


void Server::startRunning(void)
{
    interface.startListening();
    while(1) {
        string client_addr;
        auto peer_socket = interface.acceptConnections(client_addr);
        char buffer[1024];
        auto readCnt = interface.readData(buffer, peer_socket);
        printf("data%s\n", buffer);
        interface.sendDataTo(peer_socket, "ACK");
        PLOGD << "got " << buffer << endl;
    }
}

Server::Server(uint16_t port) : interface(port, true) {
    this->initializeServer();
}

Server::Server() : interface(kPort, true) {
    throw runtime_error("Server() constructor not implemented");
    this->initializeServer();

}