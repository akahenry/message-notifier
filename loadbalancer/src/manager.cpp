#include "manager.hpp"

Manager::Manager(std::string port, std::vector<server_attr> server_addresses) : server_addresses{server_addresses}
{
    this->socket = Socket(port);
}

error_t Manager::listen()
{
    this->running = true;
    return this->socket.configure(SOCKET_TYPE_SERVER);
}

void Manager::createClient(Socket client_socket)
{
    std::vector<Socket> server_sockets;
    for (size_t i = 0; i < this->server_addresses.size(); i++)
    {
        Socket socket = Socket(this->server_addresses[i].address, this->server_addresses[i].port);
        socket.configure(SOCKET_TYPE_CLIENT);
        server_sockets.push_back(socket);
        Server* server = NULL;
        // Set the first server as the master initially
        server = i == 0 ? new Server(socket, std::vector<Socket>{client_socket}) : new Server(socket);
        server->start();
        this->servers.push_back(server);
    }
    
    Client* client = new Client(client_socket, server_sockets);
    client->start();
    this->clients.push_back(client);
}

error_t Manager::serve()
{
    while(this->running)
    {
        Socket client_socket;
        error_t error = this->socket.serve(&client_socket);
        if (error)
        {
            log(LOG_TYPE_INFO, "Error accepting connection from client");
        }
        else
        {
            this->createClient(client_socket);
        }
    }

    return 0;
}