#include "session.hpp"

Session::Session(Socket _socket)
{
    *this = Session(-1, _socket);
}

Session::Session(int _id, Socket _socket) : id{_id}, socket{_socket}, running{false} {}

error_t Session::send(message_type_t type, void* message)
{
    if (type == MESSAGE_TYPE_PACKET)
    {
        return this->sendPacket(*((packet*)message));
    }
    else if (type == MESSAGE_TYPE_NOTIFICATION)
    {
        return this->sendNotification(*((notification*)message));
    }
    else
    {
        return ERROR_WRONG_MESSAGE_TYPE;
    }
}

error_t Session::sendPacket(packet pkt)
{
    return this->socket.send(pkt);
}

error_t Session::sendNotification(notification notif)
{
    return this->socket.send(notif);
}

error_t Session::listen()
{
    this->running = true;
    this->thread = std::thread(&Session::run, this);

    return 0;
}

error_t Session::run()
{
    packet pkt;

    std::cout << "DEBUG: Running session thread" << std::endl;

    while(this->running)
    {
        if (this->socket.receive(&pkt))
        {
            this->running = false;
            return ERROR_RECEIVING_MESSAGE;
        }

        if (pkt.type == PACKET_TYPE_DATA)
        {
            std::cout << "DATA: " << pkt._payload << std::endl;
        }
        else if (pkt.type == PACKET_TYPE_CMD)
        {
            std::cout << "CMD: " << pkt._payload << std::endl;
        }
    }

    return 0;
}