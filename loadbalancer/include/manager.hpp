#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "socket.hpp"
#include "queue.hpp"
#include "client.hpp"
#include "server.hpp"
#include "vector.hpp"

class Manager
{
    private:
        Socket socket;
        Vector<Client*> clients;
        Vector<Server*> servers;
        std::vector<server_attr> server_addresses;
        bool running;

        void createClient(Socket client_socket);

    public:
        Manager() = default;
        Manager(std::string port, std::vector<server_attr> server_addresses);

        error_t listen();
        error_t serve();

        static void onStopServer(Socket server_socket, Vector<Server*>* servers, Vector<Client*>* clients);
};

#endif