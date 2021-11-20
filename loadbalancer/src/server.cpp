#include "../include/server.hpp"

Server::Server(Socket listen_socket, on_stop_t on_stop) : User{listen_socket}, on_stop{on_stop} {}

Server::Server(Socket listen_socket, std::vector<Socket> publish_sockets, on_stop_t on_stop) : User{listen_socket, publish_sockets}, on_stop{on_stop} 
{
    this->listener.setAnalyzeFunction(&Server::isNotServerCloseNotification);
}

void Server::onStopListener()
{
    Socket listen_socket = this->listener.getSocket();

    this->on_stop.function(listen_socket, this->on_stop.servers, this->on_stop.clients);

    listen_socket.finish();
}

void Server::onStopPublisher()
{
    return;
}

bool Server::isNotServerCloseNotification(notification notif)
{
    return notif.error != ERROR_NOTIFICATION_SERVER_WILL_CLOSE;
}