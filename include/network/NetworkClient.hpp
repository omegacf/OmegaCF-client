#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <stdio.h> 
#include <sys/socket.h> 
#include <iostream>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <string>
#include "../util/Debug.hpp"
#include <sstream>

class NetworkClient{
    private:
        std::string _address;
        unsigned short _port;
        struct sockaddr_in _server;
        int _sock;

        NetworkClient(NetworkClient const&); // Don't Implement.
        void operator=(NetworkClient const&); // Don't implement.
    public:
        NetworkClient(std::string host, unsigned short port):_address(host), _port(port), _sock(-1){};
        int connect();
        void send(std::string message);
        std::string read(int size);
        ~NetworkClient();
};
#endif
