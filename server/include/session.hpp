#ifndef SESSION_HPP
#define SESSION_HPP

#include <thread>
#include <functional>

#include "socket.hpp"
#include "queue.hpp"

class Session
{
    private:
        Socket socket;
        std::thread thread;
        bool running;
        Queue<packet_item>* queue;

        error_t sendPacket(packet pkt);
        error_t sendNotification(notification notif);
        error_t run();

    public:
        int id;

        Session();
        Session(Socket _socket, Queue<packet_item>* _queue);
        Session(int _id, Socket _socket, Queue<packet_item>* _queue);

        error_t send(message_type_t type, void* message);
        error_t listen();
        error_t close(error_notification_t _error);
        error_t finish();
        void join();
};

#endif