#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>

#include "listener.hpp"

class Client
{
    private:
        std::string username;
        server_attr server;
        Socket socket;
        Listener listener;

    public:
        Client() = default;
        Client(std::string _user, server_attr _server);
        Client(std::string _user, std::string _address, std::string _port);

        error_t connect();
        error_t close();
        error_t send(packet_type_t type, std::string message);
};

#endif