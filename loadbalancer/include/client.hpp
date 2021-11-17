#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>

#include "socket.hpp"
#include "queue.hpp"
#include "listener.hpp"
#include "publisher.hpp"

class Client
{
    private:
        Socket client_socket;
        std::vector<Socket> server_sockets;
        Listener<packet> client_listener;
        Listener<notification> server_listener; // Only listens to master
        Publisher<packet> server_publisher;
        Publisher<notification> client_publisher;
        Queue<notification> notifications;
        Queue<packet> packets;

    public:
        Client() = default;
        Client(Socket client_socket, std::vector<Socket> server_sockets);
};

#endif