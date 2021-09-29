#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <string>
#include <mutex>

#include "user.hpp"
#include "queue.hpp"
#include "pkthandler.hpp"

class Server
{
    private:
        server_attr server;
        Socket socket;
        Queue* queue;
        PacketHandler* pkt_handler;
        bool running;

        auth_t auth(Socket socket);
    public:
        Server();
        Server(std::vector<User*> _users);
        Server(std::vector<User*> _users, std::string port);

        error_t listen();
        error_t listen(std::string port);
        error_t serve();
};

#endif