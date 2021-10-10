#include "session.hpp"

Session::Session(Socket _socket, Queue<packet_item>* _queue)
{
    *this = Session(-1, _socket, _queue);
}

Session::Session(int _id, Socket _socket, Queue<packet_item>* _queue) : id{_id}, socket{_socket}, running{false}, queue(_queue) {}

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
    log(LOG_TYPE_DEBUG,  "Starting session thread");
    this->thread = std::thread(&Session::run, this);

    return 0;
}

error_t Session::run()
{
    packet pkt;

    log(LOG_TYPE_DEBUG,  "Running session thread");

    while(this->running)
    {
        if (this->socket.receive(&pkt))
        {
            this->running = false;
            return ERROR_RECEIVING_MESSAGE;
        }

        this->queue->push({(uint16_t)this->id, pkt});
    }

    return 0;
}

error_t Session::close(error_notification_t _error)
{
    notification notif;
    notif.error = _error;

    this->socket.send(notif);
    return this->socket.finish();
}

error_t Session::finish()
{
    return this->socket.finish();
}

void Session::join()
{
    this->thread.join();
}