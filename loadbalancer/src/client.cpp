#include "client.hpp"

void Client::onStopListener()
{
    Socket listen_socket = this->listener.getSocket();
    std::vector<Socket> publish_sockets = this->publisher.getSockets();

    listen_socket.finish();
    for (size_t i = 0; i < publish_sockets.size(); i++)
    {
        publish_sockets[i].finish();
    }
}

void Client::onStopPublisher()
{
    return;
}