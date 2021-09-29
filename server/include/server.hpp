#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <mutex>

#include "user.hpp"
#include "authenticator.hpp"

class Server
{
    private:
        server_attr server;
        Socket socket;
        std::map<std::string, User*> users;
        std::mutex users_mutex;
        bool running;

    public:
        Server();
        Server(std::vector<User*> _users);
        Server(std::vector<User*> _users, std::string port);

        error_t listen();
        error_t listen(std::string port);
        error_t serve();
};

#endif