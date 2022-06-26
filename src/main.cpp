#include <iostream>
#include <string_view>
#include <vector>
#include "ncurses.hpp"
#include "server.hpp"
#include "client.hpp"

#include <arpa/inet.h>

#include <sys/socket.h>
#include <unistd.h>

string_view get_option(
    const vector<string_view>& args, 
    const string_view& option_name) {
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (*it == option_name)
            if (it + 1 != end)
                return *(it + 1);
    }
    
    return "";
}

bool has_option(
    const vector<string_view>& args, 
    const string_view& option_name) {
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (*it == option_name)
            return true;
    }
    
    return false;
}

int main(int argc, char** argv) {
    const vector<string_view> args(argv, argv + argc);

    const bool is_server = has_option(args, "-s");
    const string_view server_ip_addr = get_option(args, "-i");

    plog::init(plog::verbose, "/tmp/dummyLog.txt");
    
    if(argc < 2) {
        throw  runtime_error("no arguments passed");
        return -1;
    }
    else {
        if(is_server) {
            PLOGD << "Running in server mode";
            Server server(8111);
            server.startRunning();
        }
        else {
            string username = "alfred";
            PLOGD << "Running in client mode, " << username << " is logged in...";
            Client client(8111, "userA");
            auto ret = client.connectToServer("127.0.0.1");
            // if (ret != 0) {
            //     ret = client.sendData("fromCLIENT");
            //     assertm(ret != -1, "unable to send data to server");
            //     char buffer[1024];
            //     client.readData(buffer);
            //     cout << "Got data from sever" << buffer << endl;
                
            // }
            if(client.isConnectedToServer()) {
                // register ourselves to server
                string msg = "REG\n" + username + "\n";
                client.sendData(msg);
                char buf[1024];
                client.readData(buf);
                if(strcmp(buf, "ACK") == 0) {
                    PLOGD << "message received by server";
                }
                else
                {
                    PLOGD << "no response from server for register request";
                }
            }
            
        }
    }
    return 0;
}