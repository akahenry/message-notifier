#ifndef USER_HPP
#define USER_HPP

#include "socket.hpp"
#include "listener.hpp"
#include "publisher.hpp"
#include "queue.hpp"

template <typename T>
class User
{
    private:
        void stopListener()
        {
            this->listener.join();
            this->onStopListener();
        }
        void stopPublisher()
        {
            this->publisher.join();
            this->onStopPublisher();
        }

    protected:
        Socket listen_socket;
        std::vector<Socket> publish_sockets;
        Listener<T> listener;
        bool hasListener = false;
        Publisher<T> publisher;
        bool hasPublisher = false;
        Queue<T> events;
        std::thread listener_thread;
        std::thread publisher_thread;

        virtual void onStopListener() = 0;
        virtual void onStopPublisher() = 0;

    public:
        User() = default;
        User(Socket listen_socket) : listen_socket{listen_socket}
        {
            this->listener = Listener(this->listen_socket, &(this->events));
            this->hasListener = true;
        }
        User(std::vector<Socket> publish_sockets) : publish_sockets{publish_sockets}
        {
            this->publisher = Publisher(this->publish_sockets, &(this->events));
            this->hasPublisher = true;
        }
        User(Socket listen_socket, std::vector<Socket> publish_sockets) : listen_socket{listen_socket}, publish_sockets{publish_sockets} 
        {
            this->listener = Listener(this->listen_socket, &(this->events));
            this->hasListener = true;
            this->publisher = Publisher(this->publish_sockets, &(this->events));
            this->hasPublisher = true;
        }

        void start()
        {
            if (this->hasListener)
            {
                this->listener.start();
                this->listener_thread = std::thread(&User::stopListener, this);
            }
            if (this->hasPublisher)
            {
                this->publisher.start();
                this->publisher_thread = std::thread(&User::stopPublisher, this);
            }
        }

        void stop()
        {
            this->listener.stop();
            this->publisher.stop();
        }

        bool isRunning()
        {
            return this->listener.running && this->publisher.running;
        }
};

#endif