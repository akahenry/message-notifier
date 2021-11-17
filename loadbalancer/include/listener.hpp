#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <thread>
#include <functional>

#include "socket.hpp"
#include "queue.hpp"

template <typename T>
class Listener
{
    protected:
        Socket socket;
        Queue<T>* queue;
        std::thread thread;
        bool running = 0;

    public:
        Listener() = default;
        Listener<T>(Socket _socket, Queue<T>* _queue) : socket{_socket}, queue{_queue} {}
        
        error_t start()
        {
            this->running = true;
            this->thread = std::thread(&Listener::run, this);

            return 0;
        }

        error_t stop()
        {
            this->running = false;

            return 0;
        }

        error_t run()
        {
            T item;

            while(this->running)
            {
                if (this->socket.receive(&item))
                {
                    this->running = false;
                    return ERROR_RECEIVING_MESSAGE;
                }

                this->queue->push(item);
            }

            return 0;
        }
};

#endif