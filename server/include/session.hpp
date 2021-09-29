#ifndef SESSION_HPP
#define SESSION_HPP

#include <thread>
#include <functional>

#include "socket.hpp"

class Session
{
    private:
        int id;
        Socket socket;
        std::thread thread;
        bool running;
        std::string username;

        error_t sendPacket(packet pkt);
        error_t sendNotification(notification notif);
        error_t run();

    public:
        Session();
        Session(Socket _socket);
        Session(int _id, Socket _socket);

        error_t send(message_type_t type, void* message);
        error_t listen();
};

#endif