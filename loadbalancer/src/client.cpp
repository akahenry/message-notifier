#include "client.hpp"

void Client::onStopListener()
{
    this->listen_socket.finish();

    for (size_t i = 0; i < this->publish_sockets.size(); i++)
    {
        this->publish_sockets[i].finish();
    }
}

void Client::onStopPublisher()
{
    return;
}