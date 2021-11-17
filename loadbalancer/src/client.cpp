#include "client.hpp"

Client::Client(Socket client_socket, std::vector<Socket> server_sockets) : client_socket{client_socket}, server_sockets{server_sockets}
{
    this->server_listener = Listener(this->server_sockets[0], &(this->notifications));
    this->client_publisher = Publisher(std::vector<Socket>{this->client_socket}, &(this->notifications));
    this->client_listener = Listener(this->client_socket, &(this->packets));
    this->server_publisher = Publisher(this->server_sockets, &(this->packets));
}