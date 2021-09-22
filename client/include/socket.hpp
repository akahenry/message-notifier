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

#include "error.hpp"
#include "models.hpp"

class Socket
{
    private:
        struct sockaddr_in sockaddr;
        int sockfd;

    public:
        Socket();
        Socket(std::string _address, std::string _port);

        const int getSocket();
        error_t configure();
        error_t finish();
        error_t send(packet_type_t packet, std::string message);
        error_t receive(notification* _notification);
};