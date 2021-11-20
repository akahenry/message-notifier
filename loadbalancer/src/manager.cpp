#include <mutex>

#include "manager.hpp"

std::mutex mutex;

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
    Server::on_stop_t on_stop_server = {&Manager::onStopServer, &this->servers, &this->clients};
    for (size_t i = 0; i < this->server_addresses.size(); i++)
    {
        Socket socket = Socket(this->server_addresses[i].address, this->server_addresses[i].port);
        socket.configure(SOCKET_TYPE_CLIENT);
        server_sockets.push_back(socket);
        Server* server = NULL;
        // Set the first server as the master initially
        server = i == 0 ? new Server(socket, std::vector<Socket>{client_socket}, on_stop_server) : new Server(socket, on_stop_server);
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

void Manager::onStopServer(Socket server_socket, Vector<Server*>* servers, Vector<Client*>* clients)
{
    mutex.lock();
    std::vector<Socket> client_sockets;
    for (size_t i = 0; i < clients->size(); i++)
    {
        clients->at(i)->removePublishSocket(server_socket);
        std::vector<Socket> sockets = clients->at(i)->getPublishSockets();
        if (sockets.size() == 0)
        {
            // No more servers, push the client socket to send sync EXIT messages
            client_sockets.push_back(clients->at(i)->getListenSocket());
        }
        else
        {
            // Set new master as the next socket after the last master
            Socket new_server_socket = sockets[0];
            for (size_t j = 0; j < servers->size(); j++)
            {
                if (servers->at(j)->getListenSocket() == new_server_socket)
                {
                    servers->at(j)->addPublishSocket(clients->at(i)->getListenSocket());
                }
            }
        }
    }    

    size_t servers_length = servers->size();
    for (size_t i = 0; i < servers_length; i++)
    {
        if (servers->at(i)->getListenSocket() == server_socket)
        {
            servers->at(i)->stop();
            servers->erase(i);
        }        
    }

    for (size_t i = 0; i < client_sockets.size(); i++)
    {
        notification notif;
        notif.error = ERROR_NOTIFICATION_SERVER_WILL_CLOSE;
        client_sockets[i].send(notif);
    }
    mutex.unlock();
}