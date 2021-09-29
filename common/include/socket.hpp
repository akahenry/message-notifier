#ifndef SOCKET_HPP
#define SOCKET_HPP

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
#include <errno.h>

#include "error.hpp"
#include "models.hpp"

typedef int socket_type_t;
#define SOCKET_TYPE_CLIENT 0
#define SOCKET_TYPE_SERVER 1

class Socket
{
    private:
        struct sockaddr_in sockaddr;
        int sockfd;

        error_t configureClient();
        error_t configureServer();

    public:
        Socket() = default;
        Socket(std::string _port);
        Socket(std::string _address, std::string _port);

        const int getSocket();
        error_t configure(socket_type_t type);
        error_t finish();
        error_t send(packet_type_t packet, std::string username, std::string message);
        error_t send(packet pkt);
        error_t send(notification notif);
        error_t receive(notification* _notification);
        error_t receive(packet* pkt);
        error_t serve(Socket *socket);
};

#endif