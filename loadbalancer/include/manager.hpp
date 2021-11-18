#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <vector>

#include "socket.hpp"
#include "queue.hpp"
#include "client.hpp"
#include "server.hpp"

class Manager
{
    private:
        Socket socket;
        std::vector<Client*> clients;
        std::vector<Server*> servers;
        std::vector<server_attr> server_addresses;
        bool running;

        void createClient(Socket client_socket);

    public:
        Manager() = default;
        Manager(std::string port, std::vector<server_attr> server_addresses);

        error_t listen();
        error_t serve();
};

#endif