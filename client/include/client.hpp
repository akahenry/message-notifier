#include <iostream>

#include <user.hpp>
#include <server.hpp>
#include <error.hpp>

class Client
{
    private:
        User user;
        Server server;
    public:
        Client();
        Client(User _user, Server _server);
        Client(User _user, std::string hostname, std::string port);

        error_t connect();

};

