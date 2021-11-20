#ifndef USER_HPP
#define USER_HPP

#include <algorithm>

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
        Listener<T> listener;
        Publisher<T> publisher;
        Queue<T> events;
        std::thread listener_thread;
        std::thread publisher_thread;

        virtual void onStopListener() = 0;
        virtual void onStopPublisher() = 0;

    public:
        User() = default;
        User(Socket listen_socket)
        {
            this->listener = Listener(listen_socket, &(this->events));
            this->publisher = Publisher(std::vector<Socket>{}, &(this->events));
        }
        User(std::vector<Socket> publish_sockets)
        {
            this->listener = Listener(std::vector<Socket>{}, &(this->events));
            this->publisher = Publisher(publish_sockets, &(this->events));
        }
        User(Socket listen_socket, std::vector<Socket> publish_sockets)
        {
            this->listener = Listener(listen_socket, &(this->events));
            this->publisher = Publisher(publish_sockets, &(this->events));
        }

        void start()
        {
            this->listener.start();
            this->listener_thread = std::thread(&User::stopListener, this);
            this->publisher.start();
            this->publisher_thread = std::thread(&User::stopPublisher, this);
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

        void removePublishSocket(Socket socket)
        {
            this->publisher.removeSocket(socket);
        }

        const std::vector<Socket> getPublishSockets()
        {
            return this->publisher.getSockets();
        }

        void addPublishSocket(Socket socket)
        {
            this->publisher.addSocket(socket);
        }

        Socket getListenSocket()
        {
            return this->listener.getSocket();
        }
};

#endif