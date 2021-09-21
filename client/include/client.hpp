#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "server.hpp"
#include "error.hpp"
#include "packet.hpp"

class Client
{
    private:
        std::string username;
        server_attr server;
        struct sockaddr_in sockaddr;
        int sockfd;

    public:
        Client();
        Client(std::string _user, server_attr _server);
        Client(std::string _user, std::string _address, std::string _port);

        error_t configure();
        error_t finish();
        error_t sendMessage(packet_type_t type, std::string message);
};

